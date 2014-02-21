TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    hare.c \
    addfiles2arch.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    addfiles2arch.h

