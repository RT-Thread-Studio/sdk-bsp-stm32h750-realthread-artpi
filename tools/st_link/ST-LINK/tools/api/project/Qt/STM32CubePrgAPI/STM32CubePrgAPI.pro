#-------------------------------------------------
#
# Project created by QtCreator 2018-12-07T16:23:33
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = STM32CubePrgAPI
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += $$PWD/../../../src/CAN_Example.cpp \
    $$PWD/../../../src/I2C_Example.cpp \
    $$PWD/../../../src/SPI_Example.cpp \
    $$PWD/../../../src/UART_Example.cpp \
    $$PWD/../../../src/USB_Example.cpp \
    $$PWD/../../../src/Example1.cpp \
    $$PWD/../../../src/Example2.cpp \
    $$PWD/../../../src/Example3.cpp \
    $$PWD/../../../src/DisplayManager.cpp \
    $$PWD/../../../src/main.cpp

HEADERS += $$PWD/../../../include/CAN_Example.h \
    $$PWD/../../../include/CubeProgrammer_API.h \
    $$PWD/../../../include/DeviceDataStructure.h \
    $$PWD/../../../include/DisplayManager.h \
    $$PWD/../../../include/Example1.h \
    $$PWD/../../../include/Example2.h \
    $$PWD/../../../include/Example3.h \
    $$PWD/../../../include/I2C_Example.h \
    $$PWD/../../../include/SPI_Example.h \
    $$PWD/../../../include/UART_Example.h \
    $$PWD/../../../include/USB_Example.h \

INCLUDEPATH += "$$PWD/../../../include"

win32  {
     DESTDIR = "$$PWD/../../../lib/"
     LIBS += -L$$PWD/../../../lib/ -lCubeProgrammer_API
}

unix {
     DESTDIR = "$$PWD/../../../../bin/"
     LIBS += -L$$PWD/../../../../lib/ -lCubeProgrammer_API
     QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/.\''
     QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/../lib\''
}

macx{
     DESTDIR = "$$PWD/../../../../bin"
     LIBS += -L$$PWD/../../../../bin/ -lCubeProgrammer_API
     QMAKE_RPATHDIR += $$PWD/../../../../bin
}
