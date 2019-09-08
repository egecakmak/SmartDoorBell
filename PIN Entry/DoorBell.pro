#-------------------------------------------------
#
# Project created by QtCreator 2019-03-28T19:34:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DoorBell
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += doorbell.cpp \
            main.cpp\
            uniquelock.cpp \
            iolib.c
            #obtained from: https://www.element14.com/community/community/designcenter/single-board-computers/next-gen_beaglebone/blog/2013/10/10/bbb--beaglebone-black-io-library-for-c
            #remove "inline" for is_high and is_low


HEADERS  += doorbell.h \
            uniquelock.h \
            iolib.h
            #obtained from: https://www.element14.com/community/community/designcenter/single-board-computers/next-gen_beaglebone/blog/2013/10/10/bbb--beaglebone-black-io-library-for-c
            #remove "inline" for is_high and is_low

FORMS    += doorbell.ui

LIBS += -L/usr/lib/arm-linux-gnueabihf/ -lX11

RESOURCES += \
    images.qrc
