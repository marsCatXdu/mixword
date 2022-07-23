#include "lib/Qt-AES-1.2/qaesencryption.h"
#include "fileencryptpage.h"
#include "common/utils.h"
#include "common/cryptthread.h"

#include <QCryptographicHash>
#include <QDataStream>

#include <QTextStream>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QOperatingSystemVersion>
#include <QDesktopServices>
#include <QProcess>
#include <QDesktopWidget>
#include <QApplication>

FileEncryptPage::FileEncryptPage(QFrame *parent) : QFrame(parent)
{
    qRegisterMetaType<CryptAlg>("CryptAlg");
    qRegisterMetaType<CryptMode>("CryptMode");
    initUI();
}

void FileEncryptPage::initUI()
{
    mainlayout = new QVBoxLayout(this);
    mainlayout->setSpacing(14);
    this->setObjectName("FileEncryptPageFrame");
    this->setStyleSheet("QFrame#FileEncryptPageFrame {background-color: #FFFFFF; border-radius: 8px; padding: 4px;}");

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
    QByteArray fileByteArray = inputFile.readAll();
    inputFile.close();

    if(cryptThread!=nullptr) {
        delete cryptThread;
        cryptThread = nullptr;
    }
    cryptThread = new CryptThread(CryptAlg::AES_ECB, CryptMode::Encrypt, encryptKeySeed, fileByteArray);
    connect(cryptThread, &CryptThread::algFinished, this, &FileEncryptPage::onEncFinished);

    cryptThread->start();

    showWaitNotifyWindow();
    this->setEnabled(false);
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

    if(cryptThread!=nullptr) {
        delete cryptThread;
        cryptThread = nullptr;
    }
    cryptThread = new CryptThread(CryptAlg::AES_ECB, CryptMode::Decrypt, decryptKeySeed, cypher);
    connect(cryptThread, &CryptThread::algFinished, this, &FileEncryptPage::onDecFinished);

    cryptThread->start();

    showWaitNotifyWindow();
    this->setEnabled(false);
}

void FileEncryptPage::onEncFinished(QByteArray ret)
{
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

    hideWaitNotifyWindow();
    this->setEnabled(true);

    Utils::openExplorerAndSelectFile(outputFilename);
}

void FileEncryptPage::onDecFinished(QByteArray ret)
{
    if(ret.isEmpty()) {
        outputDecryptedFileLabel->setText(tr("DECODE ERROR!"));
        outputDecryptedFileLabel->setVisible(true);
        startDecrypt->setEnabled(false);
        return;
    }

    QString outputFilename = filenameToDec.left(filenameToDec.lastIndexOf('.'));
    QString dirPath = outputFilename.left(outputFilename.lastIndexOf('/')+1);
    QString filename = "DEC_" + outputFilename.right(outputFilename.length() - outputFilename.lastIndexOf('/')-1);

    QFile outputFile(dirPath + filename);
    if(!outputFile.open(QFile::WriteOnly)) {
        outputDecryptedFileLabel->setText(tr("SAVE FILE FAILED."));
        return;
    }
    outputFile.write(ret);
    outputFile.close();

    outputDecryptedFileLabel->setText(tr("Completed!\nOutput file:\t") + outputFilename);
    outputDecryptedFileLabel->setVisible(true);
    startDecrypt->setEnabled(false);

    hideWaitNotifyWindow();
    this->setEnabled(true);

    Utils::openExplorerAndSelectFile(outputFilename);
}

void FileEncryptPage::showWaitNotifyWindow()
{
    if(waitNotifyWindow!=nullptr) {
        delete waitNotifyWindow;
        waitNotifyWindow = nullptr;
    }
    waitNotifyWindow = new QWidget();
    waitNotifyWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    QVBoxLayout *wnLayout = new QVBoxLayout(waitNotifyWindow);
    QLabel *notifyTextLabel = new QLabel(tr("Larger files may takes longer. Please wait."));
    wnLayout->addWidget(notifyTextLabel);
    waitNotifyWindow->show();
    const QDesktopWidget *const desktop = QApplication::desktop();
    waitNotifyWindow->setFixedSize(400, 100);
    waitNotifyWindow->move((desktop->width() - waitNotifyWindow->width()) / 2, (desktop->height() - waitNotifyWindow->height()) / 2);
}

void FileEncryptPage::hideWaitNotifyWindow()
{
    waitNotifyWindow->hide();
    delete waitNotifyWindow;
    waitNotifyWindow = nullptr;
}

