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

#ifndef FINGERPRINTKEY_H
#define FINGERPRINTKEY_H

#include "keys/Key.h"

#include <QByteArray>
#include <QObject>
#include <QString>

#ifdef HAVE_FINGERPRINT
extern "C" {
#include <fprint.h>
}
#endif

class FingerprintKey : public Key
{
    Q_OBJECT

public:
    explicit FingerprintKey(QObject* parent = nullptr);
    ~FingerprintKey() override;

    bool isSupported() const;
    bool initialize();
    bool authenticate(const QString& username = QString());
    bool enroll(const QString& username = QString());
    bool deleteEnrollment(const QString& username = QString());
    bool isEnrolled(const QString& username = QString()) const;
    
    QByteArray rawKey() const override;
    QString keyFile() const override;
    bool isEmpty() const override;
    
    bool hasFingerprint() const;
    int fingerprintCount() const;
    QStringList enrolledUsers() const;

signals:
    void authenticationFinished(bool success);
    void enrollmentFinished(bool success);
    void authenticationProgress(int progress);
    void enrollmentProgress(int progress);
    void error(const QString& message);

private:
    bool initializeFingerprint();
    void cleanupFingerprint();
    bool waitForDevice();
    
#ifdef HAVE_FINGERPRINT
    struct fp_dev* m_device;
    struct fp_print_data* m_printData;
#endif
    
    QByteArray m_keyData;
    bool m_initialized;
    bool m_supported;
    bool m_enrolled;
    QString m_username;
    QStringList m_enrolledUsers;
    
    Q_DISABLE_COPY(FingerprintKey)
};

#endif // FINGERPRINTKEY_H