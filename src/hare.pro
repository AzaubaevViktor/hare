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
    listfiles.c \
    extract.c \
    findsign.c \
    decodeTree.c \
    crc.c

HEADERS += \
    hare.h \
    fileinfo.h \
    logging.h \
    init.h \
    lowfile.h \
    archfiles.h \
    path.h \
    extract.h \
    listfiles.h \
    findsign.h \
    decodeTree.h \
	crc.h

