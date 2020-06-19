QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    communicationhandler.cpp \
    databasecontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    myuser.cpp \
    newroomdialog.cpp \
    packetcontroller.cpp \
    packetwrappedtcpsocket.cpp \
    room.cpp \
    roomtreeitem.cpp \
    settings.cpp \
    user.cpp \
    usermessage.cpp \
    usertreeitem.cpp

HEADERS += \
    communicationhandler.h \
    databasecontroller.h \
    mainwindow.h \
    myuser.h \
    newroomdialog.h \
    packetcontroller.h \
    packetwrappedtcpsocket.h \
    room.h \
    roomtreeitem.h \
    settings.h \
    user.h \
    usermessage.h \
    usertreeitem.h

FORMS += \
    mainwindow.ui \
    newroomdialog.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Translations
TRANSLATIONS = en.ts  hu.ts
