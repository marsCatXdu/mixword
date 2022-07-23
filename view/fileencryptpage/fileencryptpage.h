#ifndef FILEENCRYPTPAGE_H
#define FILEENCRYPTPAGE_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QObject>

class FileEncryptPage : public QWidget
{
    Q_OBJECT
public:
    explicit FileEncryptPage(QWidget *parent = nullptr);

private:
    void initUI();

    QVBoxLayout *mainlayout = nullptr;

    QLabel *fileToBeEncrypt = nullptr;
    QPushButton *startEncrypt = nullptr;
    QLabel *outputEncryptedFileLabel = nullptr;
    QString filenameToEnc = "";          // 要被加密的文件名
    QString encryptKeySeed = "";

    QLabel *fileToBeDecrypt = nullptr;
    QPushButton *startDecrypt = nullptr;
    QLabel *outputDecryptedFileLabel = nullptr;
    QString filenameToDec = "";
    QString decryptKeySeed = "";

signals:

public slots:

private slots:
    void onToEncSelectBtnClicked();

    void encryptFile();

    void onToDecSelectBtnClicked();

    void decryptFile();

};

#endif // FILEENCRYPTPAGE_H
