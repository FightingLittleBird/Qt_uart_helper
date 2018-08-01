#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T16:50:57
#
#-------------------------------------------------

QT          += core gui
QT          += serialport
QT          += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES +=\
    mainwindow.cpp \
    main.cpp \
    usb_adapter.cpp \
    sys_public.cpp \
    dialog_pc_uart.cpp

HEADERS  += mainwindow.h \
    usb_device.h \
    usb2spi.h \
    usb2uart.h \
    usb_adapter.h \
    sys_public.h \
    dialog_pc_uart.h

FORMS    += mainwindow.ui \
    dialog_pc_uart.ui

win32{
    LIBS += -L$$PWD/lib/win32/ -lUSB2XXX
}else{
    unix:contains(QMAKE_HOST.arch, x86_64){
        LIBS += -L$$PWD/lib/linux64/ -lUSB2XXX
    }else{
        LIBS += -L$$PWD/lib/linux32 -lUSB2XXX
    }
}
!win32: DEFINES += OS_UNIX

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
