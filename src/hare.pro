TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    hare.c \
    fileinfo.c \
    getFilesInfo.c \
    addfiles2arch.c \
    lowfile.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    addfiles2arch.h \
    lowfile.h

