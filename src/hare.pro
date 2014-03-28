TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    hare.c \
    fileinfo.c \
    getFilesInfo.c \
    lowfile.c \
    archfiles.c \
    addFile.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    lowfile.h \
    archfiles.h \
    addFile.h

