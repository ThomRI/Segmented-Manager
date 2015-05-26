#ifndef ENCRYPTMANAGER_H
#define ENCRYPTMANAGER_H

#include <iostream>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QObject>

#define FAILURE false
#define SUCCESS true

class EncryptManager : public QObject
{
    Q_OBJECT

    public:
        EncryptManager(QString password);
        explicit EncryptManager(QObject *parent = 0) : QObject(parent) {}
        virtual ~EncryptManager();

        void changePassword(QString newPassword);
        bool checkMD5(QByteArray const sum);

    signals:
        void changePasswordDone(bool success);
    private:
        QString m_password = "";
        QCryptographicHash *m_passwordMD5Sum;
};

#endif // ENCRYPTMANAGER_H
