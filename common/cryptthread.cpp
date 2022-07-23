#include "cryptthread.h"
#include "lib/Qt-AES-1.2/qaesencryption.h"

#include <QCryptographicHash>

CryptThread::CryptThread(CryptAlg alg, CryptMode mode, const QString & keySeed, const QByteArray & inputByteArray, QObject *parent):
    QThread(parent),
    m_alg(alg),
    m_mode(mode),
    m_keySeed(keySeed),
    m_inputByteArray(inputByteArray)        // 原文。本线程内部会处理一步 base64 编解码问题
{
}

CryptThread::~CryptThread()
{

}

void CryptThread::run()
{
    if(m_alg==CryptAlg::AES_ECB) {
        if(m_mode==CryptMode::Encrypt)
            aesEcbEncrypt();
        if(m_mode==CryptMode::Decrypt)
            aesEcbDecrypt();
    }
}

void CryptThread::aesEcbEncrypt()
{
    QByteArray hashKey = QCryptographicHash::hash(m_keySeed.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray inputBase64 = m_inputByteArray.toBase64(QByteArray::Base64UrlEncoding);
    inputBase64.append("+++");         // 分隔符
    QByteArray ret = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::ECB, inputBase64, hashKey);

    emit algFinished(ret);
}

void CryptThread::aesEcbDecrypt()
{
    QByteArray hashKey = QCryptographicHash::hash(m_keySeed.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray dec = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::ECB, m_inputByteArray, hashKey);
    QString decStr = QString::fromLocal8Bit(dec);
    decStr = decStr.left(decStr.lastIndexOf("+++"));

    QByteArray base64Decoded = QByteArray::fromBase64(decStr.toLocal8Bit(), QByteArray::Base64UrlEncoding);

    emit algFinished(base64Decoded);
}
