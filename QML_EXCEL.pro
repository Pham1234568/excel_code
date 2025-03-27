QT       += core gui widgets charts quick qml quickcontrols2

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Cuaso.cpp \
        main.cpp

win32 {
    QXLSX_PARENTPATH = D:/Code_Qt/QXlsx/QXlsx
}
android {
    # Đối với Android, bạn có thể cần đặt lại đường dẫn thư viện nếu có
    QXLSX_PARENTPATH = $$PWD/../QXlsx/QXlsx
}
ios {
    # Tương tự, cấu hình đường dẫn cho iOS
    QXLSX_PARENTPATH = $$PWD/../QXlsx/QXlsx
}

QXLSX_HEADERPATH = $$QXLSX_PARENTPATH/header/
QXLSX_SOURCEPATH = $$QXLSX_PARENTPATH/source/
include($$QXLSX_PARENTPATH/QXlsx.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    application.qrc
    dummy.qml

HEADERS += \
    Cuaso.h

DISTFILES += \
    dummy.qml
