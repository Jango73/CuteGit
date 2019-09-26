
QT += core gui qml quick quickwidgets quickcontrols2 svg network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DEFINES += "VERSION_STRING=\"\\\"0.0.0.beta.1\\\"\""
DEFINES += QTPLUS_LIBRARY

# Dependencies
INCLUDEPATH += $$PWD/../qt-plus/source/cpp

# Sources
include(CuteGit.pri)

# Directories
DESTDIR = $$OUT_PWD/bin

# Target
CONFIG(debug, debug|release) {
    unix {
        TARGET = CuteGitApp-dbg
    } else {
        TARGET = CuteGit-dbg
    }
} else {
    unix {
        TARGET = CuteGitApp
    } else {
        TARGET = CuteGit
    }
}

# Deployment
#TEMP_NAME = $${QMAKE_QMAKE}
#QT_PATH = $$dirname(TEMP_NAME)
#QT_LIB_PATH = $$absolute_path($${QT_PATH}/../lib)

#defineTest(copyFilesToDir) {
#    sourceDir = $$1
#    files = $$2
#    targetDir = $$3
#    win32:targetDir ~= s,/,\\,g

#    for(file, files) {
#        final = $$sourceDir/$$file
#        win32:final ~= s,/,\\,g

#        INSTALLS += $$QMAKE_COPY $$shell_quote($$final) $$shell_quote($$targetDir) $$escape_expand(\\n\\t)
#    }
#}

#copyFilesToDir($$QT_LIB_PATH, $$QT_LIB_NAMES, $$DESTDIR)
