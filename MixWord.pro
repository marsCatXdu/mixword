#-------------------------------------------------
#
# Project created by QtCreator 2022-07-17T13:44:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MixWord
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        lib/Qt-AES-1.2/qaesencryption.cpp \
        main.cpp \
        mainwindow.cpp \
        view/tab.cpp \
        view/fileencryptpage/fileencryptpage.cpp \
    view/textencryptpage/textencryptpage.cpp \
    common/utils.cpp \
    common/ddestylemenubutton.cpp \
    common/cryptthread.cpp

HEADERS += \
        lib/Qt-AES-1.2/aesni/aesni-enc-cbc.h \
        lib/Qt-AES-1.2/aesni/aesni-enc-ecb.h \
        lib/Qt-AES-1.2/aesni/aesni-key-exp.h \
        lib/Qt-AES-1.2/qaesencryption.h \
        mainwindow.h \
        view/tab.h \
        view/fileencryptpage/fileencryptpage.h \
    view/textencryptpage/textencryptpage.h \
    common/utils.h \
    common/ddestylemenubutton.h \
    common/cryptthread.h

TRANSLATIONS = i18n/zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    translations.qrc
