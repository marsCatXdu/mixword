#ifndef CRYPTTHREAD_H
#define CRYPTTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>

enum CryptAlg {
    AES_ECB,
};

enum CryptMode {
    Encrypt,
    Decrypt
};

class CryptThread : public QThread
{
    Q_OBJECT
public:
    CryptThread(CryptAlg alg, CryptMode mode, const QString & keySeed, const QByteArray & inputByteArray, QObject *parent = nullptr);
    ~CryptThread();

signals:
    void algFinished(QByteArray ret);

protected:
    void run();

private:
    void aesEcbEncrypt();

    void aesEcbDecrypt();

    CryptAlg m_alg;
    CryptMode m_mode;

    QString m_keySeed;
    QByteArray m_inputByteArray;

};

#endif // CRYPTTHREAD_H
