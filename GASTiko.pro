#-------------------------------------------------
#
# Project created by QtCreator 2019-03-09T08:51:21
#
#-------------------------------------------------

QT       += core gui sql printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = res.rc

TARGET = GASTiko
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


SOURCES += \
    discountcard.cpp \
        main.cpp \
        maindialog.cpp \
    database.cpp \
    working.cpp \
    config.cpp \
    c5tablewidget.cpp \
    c5lineedit.cpp \
    c5checkbox.cpp \
    c5combobox.cpp \
    c5dateedit.cpp \
    c5utils.cpp \
    excel.cpp \
    datedialog.cpp \
    configdialog.cpp

HEADERS += \
    discountcard.h \
        maindialog.h \
    database.h \
    working.h \
    config.h \
    c5tablewidget.h \
    c5lineedit.h \
    c5checkbox.h \
    c5combobox.h \
    c5dateedit.h \
    c5utils.h \
    excel.h \
    datedialog.h \
    rc.h \
    configdialog.h

FORMS += \
    discountcard.ui \
        maindialog.ui \
    working.ui \
    datedialog.ui \
    configdialog.ui

RESOURCES += \
    res.qrc
