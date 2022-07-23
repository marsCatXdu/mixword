#include "utils.h"

#include <QDir>
#include <QUrl>
#include <QOperatingSystemVersion>
#include <QDesktopServices>
#include <QProcess>

Utils::Utils(QWidget *parent) : QWidget(parent)
{

}

void Utils::openExplorerAndSelectFile(const QString & selectFilename)
{
    if(QOperatingSystemVersion::currentType()==QOperatingSystemVersion::Windows) {  // Windows
        QProcess::startDetached(QString("explorer.exe /select, ") + QDir::toNativeSeparators(selectFilename));
    } else {                                                                        // *nix
        QDesktopServices::openUrl(QUrl("file:///"+selectFilename.left(selectFilename.lastIndexOf('/'))));
    }
}
