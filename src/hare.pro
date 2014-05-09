TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    hare.c \
    init.c \
    helper.c \
    fileinfo.c \
    getFilesInfo.c \
    lowfile.c \
    archfiles.c \
    addFile.c \
    huff.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    init.h \
    lowfile.h \
    archfiles.h \
    addFile.h \
    huff.h

