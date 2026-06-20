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

#include "FingerprintKey.h"
#include "core/Global.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QUuid>

#ifdef HAVE_FINGERPRINT
extern "C" {
#include <fprint.h>
}
#endif

FingerprintKey::FingerprintKey(QObject* parent)
    : Key(parent)
#ifdef HAVE_FINGERPRINT
    , m_device(nullptr)
    , m_printData(nullptr)
#endif
    , m_initialized(false)
    , m_supported(false)
    , m_enrolled(false)
{
    m_supported = initializeFingerprint();
}

FingerprintKey::~FingerprintKey()
{
    cleanupFingerprint();
}

bool FingerprintKey::isSupported() const
{
    return m_supported;
}

bool FingerprintKey::initialize()
{
    if (!m_supported) {
        return false;
    }
    
    if (m_initialized) {
        return true;
    }
    
#ifdef HAVE_FINGERPRINT
    if (!waitForDevice()) {
        emit error(tr("Fingerprint device not found"));
        return false;
    }
    
    m_initialized = true;
    return true;
#else
    return false;
#endif
}

bool FingerprintKey::authenticate(const QString& username)
{
    if (!m_initialized) {
        if (!initialize()) {
            return false;
        }
    }
    
#ifdef HAVE_FINGERPRINT
    if (!m_device) {
        emit error(tr("Fingerprint device not initialized"));
        return false;
    }
    
    // Load stored fingerprint data
    QString storePath = getFingerprintStorePath(username);
    if (!QFile::exists(storePath)) {
        emit error(tr("No fingerprint enrolled for user: %1").arg(username));
        return false;
    }
    
    // Read stored fingerprint data
    QFile file(storePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Failed to read fingerprint data"));
        return false;
    }
    
    QByteArray storedData = file.readAll();
    file.close();
    
    // Convert to fingerprint data structure
    struct fp_print_data* storedPrint = fp_print_data_from_data(
        reinterpret_cast<const unsigned char*>(storedData.constData()),
        storedData.size()
    );
    
    if (!storedPrint) {
        emit error(tr("Invalid fingerprint data"));
        return false;
    }
    
    // Start verification
    int result = fp_verify_finger_img(m_device, storedPrint, nullptr);
    
    // Clean up
    fp_print_data_free(storedPrint);
    
    if (result == FP_VERIFY_MATCH) {
        m_username = username;
        m_keyData = generateKeyFromFingerprint();
        emit authenticationFinished(true);
        return true;
    } else if (result == FP_VERIFY_NO_MATCH) {
        emit error(tr("Fingerprint does not match"));
        emit authenticationFinished(false);
        return false;
    } else if (result == FP_VERIFY_RETRY) {
        emit error(tr("Fingerprint scan failed, please try again"));
        emit authenticationFinished(false);
        return false;
    } else {
        emit error(tr("Fingerprint verification failed with error: %1").arg(result));
        emit authenticationFinished(false);
        return false;
    }
#else
    Q_UNUSED(username)
    emit error(tr("Fingerprint authentication not supported on this platform"));
    return false;
#endif
}

bool FingerprintKey::enroll(const QString& username)
{
    if (!m_initialized) {
        if (!initialize()) {
            return false;
        }
    }
    
#ifdef HAVE_FINGERPRINT
    if (!m_device) {
        emit error(tr("Fingerprint device not initialized"));
        return false;
    }
    
    // Start enrollment
    int result = fp_enroll_finger_img(m_device, &m_printData, nullptr);
    
    if (result == FP_ENROLL_COMPLETE) {
        // Save enrolled fingerprint data
        if (saveFingerprintData(username, m_printData)) {
            m_enrolled = true;
            m_username = username;
            m_enrolledUsers.append(username);
            emit enrollmentFinished(true);
            return true;
        } else {
            emit error(tr("Failed to save fingerprint data"));
            emit enrollmentFinished(false);
            return false;
        }
    } else if (result == FP_ENROLL_RETRY) {
        emit error(tr("Fingerprint enrollment failed, please try again"));
        emit enrollmentProgress(0);
        emit enrollmentFinished(false);
        return false;
    } else if (result == FP_ENROLL_RETRY_TOO_SHORT) {
        emit error(tr("Fingerprint scan too short, please try again"));
        emit enrollmentProgress(0);
        emit enrollmentFinished(false);
        return false;
    } else if (result == FP_ENROLL_RETRY_CENTER_FINGER) {
        emit error(tr("Please center your finger on the sensor"));
        emit enrollmentProgress(0);
        emit enrollmentFinished(false);
        return false;
    } else if (result == FP_ENROLL_RETRY_REMOVE_FINGER) {
        emit error(tr("Please remove your finger from the sensor"));
        emit enrollmentProgress(0);
        emit enrollmentFinished(false);
        return false;
    } else {
        emit error(tr("Fingerprint enrollment failed with error: %1").arg(result));
        emit enrollmentProgress(0);
        emit enrollmentFinished(false);
        return false;
    }
#else
    Q_UNUSED(username)
    emit error(tr("Fingerprint enrollment not supported on this platform"));
    return false;
#endif
}

bool FingerprintKey::deleteEnrollment(const QString& username)
{
    QString storePath = getFingerprintStorePath(username);
    if (QFile::exists(storePath)) {
        if (QFile::remove(storePath)) {
            m_enrolledUsers.removeAll(username);
            if (m_username == username) {
                m_username.clear();
                m_keyData.clear();
                m_enrolled = false;
            }
            return true;
        }
    }
    return false;
}

bool FingerprintKey::isEnrolled(const QString& username) const
{
    QString storePath = getFingerprintStorePath(username);
    return QFile::exists(storePath);
}

QByteArray FingerprintKey::rawKey() const
{
    return m_keyData;
}

QString FingerprintKey::keyFile() const
{
    if (m_username.isEmpty()) {
        return QString();
    }
    return getFingerprintStorePath(m_username);
}

bool FingerprintKey::isEmpty() const
{
    return m_keyData.isEmpty();
}

bool FingerprintKey::hasFingerprint() const
{
    return m_supported && m_initialized;
}

int FingerprintKey::fingerprintCount() const
{
    return m_enrolledUsers.size();
}

QStringList FingerprintKey::enrolledUsers() const
{
    return m_enrolledUsers;
}

bool FingerprintKey::initializeFingerprint()
{
#ifdef HAVE_FINGERPRINT
    // Initialize libfprint
    int result = fp_init();
    if (result < 0) {
        return false;
    }
    
    // Check for available devices
    struct fp_dscv_dev** devices = fp_discover_devs();
    if (!devices || !devices[0]) {
        fp_exit();
        return false;
    }
    
    // Get the first available device
    m_device = fp_dev_open(devices[0]);
    fp_dscv_devs_free(devices);
    
    if (!m_device) {
        fp_exit();
        return false;
    }
    
    // Load enrolled users
    loadEnrolledUsers();
    
    return true;
#else
    return false;
#endif
}

void FingerprintKey::cleanupFingerprint()
{
#ifdef HAVE_FINGERPRINT
    if (m_printData) {
        fp_print_data_free(m_printData);
        m_printData = nullptr;
    }
    
    if (m_device) {
        fp_dev_close(m_device);
        m_device = nullptr;
    }
    
    fp_exit();
#endif
    m_initialized = false;
    m_supported = false;
}

bool FingerprintKey::waitForDevice()
{
#ifdef HAVE_FINGERPRINT
    if (!m_device) {
        return false;
    }
    
    // Check if device supports fingerprint scanning
    return fp_dev_get_nr_enroll_stages(m_device) > 0;
#else
    return false;
#endif
}

QString FingerprintKey::getFingerprintStorePath(const QString& username) const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fingerprintDir = dataPath + "/fingerprints";
    
    QDir dir(fingerprintDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    QString filename = username.isEmpty() ? "default.fprint" : username + ".fprint";
    return fingerprintDir + "/" + filename;
}

bool FingerprintKey::saveFingerprintData(const QString& username, struct fp_print_data* data)
{
#ifdef HAVE_FINGERPRINT
    if (!data) {
        return false;
    }
    
    size_t dataSize;
    unsigned char* serializedData = fp_print_data_get_data(data, &dataSize);
    if (!serializedData) {
        return false;
    }
    
    QString storePath = getFingerprintStorePath(username);
    QFile file(storePath);
    if (!file.open(QIODevice::WriteOnly)) {
        fp_print_data_free_data(serializedData);
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(serializedData), dataSize);
    file.close();
    
    fp_print_data_free_data(serializedData);
    return true;
#else
    Q_UNUSED(username)
    Q_UNUSED(data)
    return false;
#endif
}

void FingerprintKey::loadEnrolledUsers()
{
    m_enrolledUsers.clear();
    
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fingerprintDir = dataPath + "/fingerprints";
    
    QDir dir(fingerprintDir);
    if (!dir.exists()) {
        return;
    }
    
    QStringList filters;
    filters << "*.fprint";
    
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);
    for (const QFileInfo& fileInfo : fileList) {
        QString username = fileInfo.baseName();
        if (username != "default") {
            m_enrolledUsers.append(username);
        }
    }
    
    // Check if default fingerprint exists
    if (QFile::exists(fingerprintDir + "/default.fprint")) {
        if (!m_enrolledUsers.contains("default")) {
            m_enrolledUsers.prepend("default");
        }
    }
}

QByteArray FingerprintKey::generateKeyFromFingerprint() const
{
    // Generate a deterministic key from fingerprint data
    // This is a simplified implementation - in production, you'd want
    // to use a proper key derivation function
    QByteArray seed = "KeePassXC-Fingerprint-" + m_username.toUtf8();
    return QCryptographicHash::hash(seed, QCryptographicHash::Sha256);
}

FILE: src/keys/drivers/FingerprintKey_p.h
ACTION: create
CONTENT: