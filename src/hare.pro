TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    hare.c \
    init.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    init.h

