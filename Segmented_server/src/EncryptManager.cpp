#include "EncryptManager.h"

EncryptManager::EncryptManager(QString password)
{
    m_passwordMD5Sum = new QCryptographicHash(QCryptographicHash::Md5);
    changePassword(password);
}

/*!
    \brief Changes the password in memory, and generates its MD5 sum
*/
void EncryptManager::changePassword(QString newPassword)
{
    //out::information("Changing password...");
    m_passwordMD5Sum->reset();
    m_passwordMD5Sum->addData(newPassword.toUtf8());
    m_password = newPassword;
    emit changePasswordDone(SUCCESS);

    // OLD SECURITY, DOESN'T WORK!
    /// TODO Put the change password security back in place ! (EncryptManager)
    /*if(m_passwordMD5Sum->addData(newPassword.toUtf8())) { // If the calculation of the MD5 sum didn't fail
        m_password = newPassword;
        emit changePasswordDone(SUCCESS);
    } else { // Otherwise,
        emit changePasswordDone(FAILURE);
        changePassword(m_password); // We recalculate the previous sum.
        // /!\ If the password change fails everytime, the server will be stuck on this (infinite loop).
    }*/
}

bool EncryptManager::checkMD5(QByteArray const sum)
{
    return m_passwordMD5Sum->result() == sum;
}

EncryptManager::~EncryptManager()
{
    //dtor
}
