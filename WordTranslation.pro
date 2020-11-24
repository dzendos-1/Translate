QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = "qt-logo.ico"

LIBS += -lkernel32 -luser32

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mywords.cpp

HEADERS += \
    mainwindow.h \
    mywords.h

FORMS += \
    mainwindow.ui \
    mywords.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    JQuery.qrc

DISTFILES +=
