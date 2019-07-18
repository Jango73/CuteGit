
QT += core gui qml quick quickwidgets quickcontrols2 svg network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DEFINES += QTPLUS_LIBRARY

# Dependencies
INCLUDEPATH += $$PWD/../qt-plus/source/cpp

# Sources
include(CuteGit.pri)

# Directories
DESTDIR = $$PWD/bin

# Target
CONFIG(debug, debug|release) {
    TARGET = cutegitd
} else {
    TARGET = cutegit
}
