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
    path.c \
    extract.c \
    crc.c \
    addFile.c \
    huff.c \
    concat_string.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    init.h \
    lowfile.h \
    archfiles.h \
    path.h \
    extract.h \
    crc.h \
    addFile.h \
    huff.h \
    concat_string.h

