#include "lib/Qt-AES-1.2/qaesencryption.h"
#include "fileencryptpage.h"
#include "common/utils.h"

#include <QCryptographicHash>
#include <QDataStream>

#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QOperatingSystemVersion>
#include <QDesktopServices>
#include <QProcess>

FileEncryptPage::FileEncryptPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void FileEncryptPage::initUI()
{
    mainlayout = new QVBoxLayout(this);

    QHBoxLayout *fileSelectLayout = new QHBoxLayout();
    QLabel *encryptTitle = new QLabel(tr("Encrypt File"));
    fileToBeEncrypt = new QLabel(tr("Click button to select file to be encrypt"));
    QPushButton *fileSelectBtn = new QPushButton(tr("Select File"));
    connect(fileSelectBtn, &QPushButton::clicked, this, &FileEncryptPage::onToEncSelectBtnClicked);
    fileSelectLayout->addWidget(fileToBeEncrypt);
    fileSelectLayout->addStretch();
    fileSelectLayout->addWidget(fileSelectBtn);

    encryptKeySeed = "1234567";
    QHBoxLayout *encKeySeedLayout = new QHBoxLayout();
    QLabel *encKeySeedLabel = new QLabel(tr("Key seed: "));
    QLineEdit *encKeySeedEdit = new QLineEdit();
    encKeySeedEdit->setText(encryptKeySeed);
    encKeySeedLayout->addWidget(encKeySeedLabel);
    encKeySeedLayout->addWidget(encKeySeedEdit);
    connect(encKeySeedEdit, &QLineEdit::textChanged, this, [=](const QString & newText){ encryptKeySeed = newText; });

    startEncrypt = new QPushButton(tr("Encrypt File"));
    connect(startEncrypt, &QPushButton::clicked, this, &FileEncryptPage::encryptFile);
    startEncrypt->setEnabled(false);

    outputEncryptedFileLabel = new QLabel(this);
    outputEncryptedFileLabel->setVisible(false);

    QHBoxLayout *decFileSelectLayout = new QHBoxLayout();
    QLabel *decryptTitle = new QLabel(tr("Decrypt File"));
    fileToBeDecrypt = new QLabel(tr("Click button to select file to be decrypt"));
    QPushButton *decFileSelectBtn = new QPushButton(tr("Select File"));
    connect(decFileSelectBtn, &QPushButton::clicked, this, &FileEncryptPage::onToDecSelectBtnClicked);
    decFileSelectLayout->addWidget(fileToBeDecrypt);
    decFileSelectLayout->addStretch();
    decFileSelectLayout->addWidget(decFileSelectBtn);

    decryptKeySeed = "1234567";
    QHBoxLayout *decKeySeedLayout = new QHBoxLayout();
    QLabel *decKeySeedLabel = new QLabel(tr("Key seed: "));
    QLineEdit *decKeySeedEdit = new QLineEdit();
    decKeySeedEdit->setText(decryptKeySeed);
    decKeySeedLayout->addWidget(decKeySeedLabel);
    decKeySeedLayout->addWidget(decKeySeedEdit);
    connect(decKeySeedEdit, &QLineEdit::textChanged, this, [=](const QString & newText){ decryptKeySeed = newText; });

    startDecrypt = new QPushButton(tr("Decrypt File"));
    connect(startDecrypt, &QPushButton::clicked, this, &FileEncryptPage::decryptFile);
    startDecrypt->setEnabled(false);

    outputDecryptedFileLabel = new QLabel(this);
    outputDecryptedFileLabel->setVisible(false);

    mainlayout->addWidget(encryptTitle);
    mainlayout->addLayout(fileSelectLayout);
    mainlayout->addLayout(encKeySeedLayout);
    mainlayout->addWidget(outputEncryptedFileLabel);
    mainlayout->addWidget(startEncrypt);
    mainlayout->addSpacing(30);
    mainlayout->addWidget(decryptTitle);
    mainlayout->addLayout(decKeySeedLayout);
    mainlayout->addLayout(decFileSelectLayout);
    mainlayout->addWidget(outputDecryptedFileLabel);
    mainlayout->addWidget(startDecrypt);
    mainlayout->addStretch();
}

void FileEncryptPage::onToEncSelectBtnClicked()
{
    filenameToEnc = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());
    if(filenameToEnc.isEmpty()) return;
    fileToBeEncrypt->setText(tr("Select file:\t") + filenameToEnc);
    outputEncryptedFileLabel->setVisible(false);
    outputEncryptedFileLabel->setText("");
    startEncrypt->setEnabled(true);
}

void FileEncryptPage::encryptFile()
{
    QFile inputFile(filenameToEnc);
    if(!inputFile.open(QIODevice::ReadOnly)) {
        outputEncryptedFileLabel->setText(tr("SELECTED FILE OPEN FAILED!"));
        outputEncryptedFileLabel->setVisible(true);
        return;
    }
    QByteArray base64File = inputFile.readAll().toBase64(QByteArray::Base64UrlEncoding);
    inputFile.close();

    base64File.append("+++");         // 分隔符

    QByteArray hashKey = QCryptographicHash::hash(encryptKeySeed.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray ret = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::ECB, base64File, hashKey);

    QString outputFilename = filenameToEnc + ".lraes256ecbenc";
    QFile outputFile(outputFilename);
    if(!outputFile.open(QFile::WriteOnly)) {
        return;
    }
    outputFile.write(ret);
    outputFile.close();

    outputEncryptedFileLabel->setText(tr("Completed!\nOutput file:\t") + outputFilename);
    outputEncryptedFileLabel->setVisible(true);

    startEncrypt->setEnabled(false);

    Utils::openExplorerAndSelectFile(outputFilename);
}

void FileEncryptPage::onToDecSelectBtnClicked()
{
    filenameToDec = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());
    if(filenameToDec.isEmpty()) {
        return;
    }
    fileToBeDecrypt->setText(tr("Select file:\t") + filenameToDec);
    outputDecryptedFileLabel->setVisible(false);
    startDecrypt->setEnabled(true);
}

void FileEncryptPage::decryptFile()
{
    QFile inputFile(filenameToDec);
    if(!inputFile.open(QIODevice::ReadOnly)) {
        outputDecryptedFileLabel->setText(tr("SELECTED FILE OPEN FAILED!"));
        outputDecryptedFileLabel->setVisible(true);
        return;
    }
    QByteArray cypher = inputFile.readAll();
    inputFile.close();

    QByteArray hashKey = QCryptographicHash::hash(decryptKeySeed.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray dec = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::ECB, cypher, hashKey);
    if(dec.isEmpty()) {
        outputDecryptedFileLabel->setText(tr("DECODE ERROR!"));
        outputDecryptedFileLabel->setVisible(true);
        startDecrypt->setEnabled(false);
        return;
    }
    QString decStr = QString::fromLocal8Bit(dec);
    decStr = decStr.left(decStr.lastIndexOf("+++"));

    QByteArray base64Decoded = QByteArray::fromBase64(decStr.toLocal8Bit(), QByteArray::Base64UrlEncoding);

    QString outputFilename = filenameToDec.left(filenameToDec.lastIndexOf('.'));
    QString dirPath = outputFilename.left(outputFilename.lastIndexOf('/')+1);
    QString filename = "DEC_" + outputFilename.right(outputFilename.length() - outputFilename.lastIndexOf('/')-1);

    QFile outputFile(dirPath + filename);
    if(!outputFile.open(QFile::WriteOnly)) {
        outputDecryptedFileLabel->setText(tr("SAVE FILE FAILED."));
        return;
    }
    outputFile.write(base64Decoded);
    outputFile.close();

    outputDecryptedFileLabel->setText(tr("Completed!\nOutput file:\t") + outputFilename);
    outputDecryptedFileLabel->setVisible(true);
    startDecrypt->setEnabled(false);

    Utils::openExplorerAndSelectFile(outputFilename);
}
