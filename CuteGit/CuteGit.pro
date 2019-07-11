
QT += core gui qml quick quickwidgets svg network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# Shadow build flag
!equals($${PWD}, $${OUT_PWD}) {
    shadow = 1
} else {
    shadow = 0
}

# Dependencies
INCLUDEPATH += $$PWD/../qt-plus/source/cpp

# Sources
include(CuteGit.pri)

# Directories
DESTDIR = $$PWD/bin

equals($${shadow}, 0) {
    MOC_DIR = $$PWD/build/moc
    OBJECTS_DIR = $$PWD/build/obj
    RCC_DIR = $$PWD/build/rcc
    UI_DIR = $$PWD/build/ui
}

# Target
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../qt-plus/bin -lqt-plusd
    TARGET = cutegitd
} else {
    LIBS += -L$$PWD/../qt-plus/bin -lqt-plus
    TARGET = cutegit
}

SUBDIRS += qt-plus

#exists($$DESTDIR) {
#} else {
#    QMAKE_PRE_LINK += $$QMAKE_MKDIR $$quote($$shell_path($${DESTDIR}) $$escape_expand(\\n\\t))
#}

# Copy qt-plus to bin
copyfile = $$PWD/../qt-plus/bin/*
copydest = $$DESTDIR

QMAKE_PRE_LINK += $$QMAKE_COPY $$quote($$shell_path($$copyfile)) $$quote($$shell_path($$copydest)) $$escape_expand(\\n\\t)
