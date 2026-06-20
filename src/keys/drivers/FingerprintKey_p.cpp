/*
 *  Copyright (C) 2024 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FingerprintKey_p.h"

#include <QDebug>

#ifdef HAVE_FINGERPRINT
// Callback functions for libfprint
static void verify_cb(struct fp_dev* dev, int result, struct fp_print_data* match, struct fp_img* img, void* user_data)
{
    Q_UNUSED(dev)
    Q_UNUSED(match)
    Q_UNUSED(img)
    
    FingerprintKeyPrivate* d = static_cast<FingerprintKeyPrivate*>(user_data);
    d->handleVerifyResult(result);
}

static void enroll_cb(struct fp_dev* dev, int result, struct fp_print_data* print_data, struct fp_img* img, void* user_data)
{
    Q_UNUSED(dev)
    Q_UNUSED(img)
    
    FingerprintKeyPrivate* d = static_cast<FingerprintKeyPrivate*>(user_data);
    d->handleEnrollResult(result, print_data);
}

static void img_cb(struct fp_dev* dev, struct fp_img* img, void* user_data)
{
    Q_UNUSED(dev)
    
    FingerprintKeyPrivate* d = static_cast<FingerprintKeyPrivate*>(user_data);
    d->handleImageCaptured(img);
}
#endif

FingerprintKeyPrivate::FingerprintKeyPrivate()
#ifdef HAVE_FINGERPRINT
    : m_device(nullptr)
    , m_printData(nullptr)
    , m_image(nullptr)
#endif
    , m_initialized(false)
    , m_operationInProgress(false)
    , m_enrollStage(0)
    , m_totalEnrollStages(0)
{
}

FingerprintKeyPrivate::~FingerprintKeyPrivate()
{
    cleanupDevice();
}

bool FingerprintKeyPrivate::initializeDevice()
{
    if (m_initialized) {
        return true;
    }
    
#ifdef HAVE_FINGERPRINT
    // Initialize libfprint
    int result = fp_init();
    if (result < 0) {
        qWarning() << "Failed to initialize libfprint:" << result;
        return false;
    }
    
    // Discover devices
    struct fp_dscv_dev** devices = fp_discover_devs();
    if (!devices || !devices[0]) {
        qWarning() << "No fingerprint devices found";
        fp_exit();
        return false;
    }
    
    // Open the first available device
    m_device = fp_dev_open(devices[0]);
    fp_dscv_devs_free(devices);
    
    if (!m_device) {
        qWarning() << "Failed to open fingerprint device";
        fp_exit();
        return false;
    }
    
    // Get device capabilities
    m_totalEnrollStages = fp_dev_get_nr_enroll_stages(m_device);
    if (m_totalEnrollStages <= 0) {
        qWarning() << "Device does not support fingerprint enrollment";
        fp_dev_close(m_device);
        m_device = nullptr;
        fp_exit();
        return false;
    }
    
    setupCallbacks();
    
    m_initialized = true;
    return true;
#else
    return false;
#endif
}

void FingerprintKeyPrivate::cleanupDevice()
{
#ifdef HAVE_FINGERPRINT
    if (m_operationInProgress) {
        cancelOperation();
    }
    
    if (m_printData) {
        fp_print_data_free(m_printData);
        m_printData = nullptr;
    }
    
    if (m_image) {
        fp_img_free(m_image);
        m_image = nullptr;
    }
    
    if (m_device) {
        fp_dev_close(m_device);
        m_device = nullptr;
    }
    
    if (m_initialized) {
        fp_exit();
    }
#endif
    
    m_initialized = false;
    m_enrollStage = 0;
}

bool FingerprintKeyPrivate::startVerification(struct fp_print_data* storedPrint)
{
    if (!m_initialized || !m_device || m_operationInProgress) {
        return false;
    }
    
#ifdef HAVE_FINGERPRINT
    m_operationInProgress = true;
    
    int result = fp_verify_finger_start(m_device, storedPrint, verify_cb, this);
    if (result < 0) {
        qWarning() << "Failed to start fingerprint verification:" << result;
        m_operationInProgress = false;
        return false;
    }
    
    return true;
#else
    Q_UNUSED(storedPrint)
    return false;
#endif
}

bool FingerprintKeyPrivate::startEnrollment()
{
    if (!m_initialized || !m_device || m_operationInProgress) {
        return false;
    }
    
#ifdef HAVE_FINGERPRINT
    m_operationInProgress = true;
    m_enrollStage = 0;
    
    int result = fp_enroll_finger_start(m_device, enroll_cb, this);
    if (result < 0) {
        qWarning() << "Failed to start fingerprint enrollment:" << result;
        m_operationInProgress = false;
        return false;
    }
    
    return true;
#else
    return false;
#endif
}

void FingerprintKeyPrivate::cancelOperation()
{
#ifdef HAVE_FINGERPRINT
    if (!m_operationInProgress || !m_device) {
        return;
    }
    
    fp_verify_finger_stop(m_device);
    fp_enroll_finger_stop(m_device);
    
    m_operationInProgress = false;
#endif
}

bool FingerprintKeyPrivate::isDeviceReady() const
{
    return m_initialized && m_device != nullptr;
}

bool FingerprintKeyPrivate::hasFingerprintReader() const
{
#ifdef HAVE_FINGERPRINT
    return m_device != nullptr && fp_dev_get_nr_enroll_stages(m_device) > 0;
#else
    return false;
#endif
}

int FingerprintKeyPrivate::getEnrollStages() const
{
    return m_totalEnrollStages;
}

QString FingerprintKeyPrivate::getDeviceName() const
{
#ifdef HAVE_FINGERPRINT
    if (!m_device) {
        return QString();
    }
    
    const char* name = fp_driver_get_full_name(fp_dev_get_driver(m_device));
    return name ? QString::fromUtf8(name) : QString();
#else
    return QString();
#endif
}

void FingerprintKeyPrivate::handleVerifyResult(int result)
{
#ifdef HAVE_FINGERPRINT
    m_operationInProgress = false;
    
    bool success = (result == FP_VERIFY_MATCH);
    
    if (m_verifyCallback) {
        m_verifyCallback(success);
    }
#endif
}

void FingerprintKeyPrivate::handleEnrollResult(int result, struct fp_print_data* data)
{
#ifdef HAVE_FINGERPRINT
    if (result == FP_ENROLL_COMPLETE) {
        m_operationInProgress = false;
        m_printData = data;
        
        if (m_enrollCallback) {
            m_enrollCallback(true, data);
        }
    } else if (result == FP_ENROLL_FAIL || result == FP_ENROLL_RETRY || 
               result == FP_ENROLL_RETRY_TOO_SHORT || result == FP_ENROLL_RETRY_CENTER_FINGER ||
               result == FP_ENROLL_RETRY_REMOVE_FINGER) {
        m_enrollStage++;
        
        if (m_enrollCallback) {
            m_enrollCallback(false, nullptr);
        }
        
        // If we've completed all stages, we're done
        if (m_enrollStage >= m_totalEnrollStages) {
            m_operationInProgress = false;
        }
    } else {
        m_operationInProgress = false;
        
        if (m_enrollCallback) {
            m_enrollCallback(false, nullptr);
        }
    }
#endif
}

void FingerprintKeyPrivate::handleImageCaptured(struct fp_img* img)
{
#ifdef HAVE_FINGERPRINT
    if (m_image) {
        fp_img_free(m_image);
    }
    m_image = img;
#endif
}

void FingerprintKeyPrivate::setupCallbacks()
{
#ifdef HAVE_FINGERPRINT
    // The callbacks are set when starting operations
#endif
}

FILE: src/gui/dbsettings/DatabaseSettingsWidgetFingerprint.h
ACTION: create
CONTENT: