QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui
QXLSX_PARENTPATH = D:/Code_Qt/QXlsx/QXlsx
QXLSX_HEADERPATH = $$QXLSX_PARENTPATH/header/
QXLSX_SOURCEPATH = $$QXLSX_PARENTPATH/source/
include($$QXLSX_PARENTPATH/QXlsx.pri)

# Quy tắc cài đặt mặc định
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DEFINES += PROJECT_PATH=\\\"$$PWD\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
