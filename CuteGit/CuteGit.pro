
QT += core gui qml quick quickwidgets quickcontrols2 network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
DEFINES += "VERSION_STRING=\"\\\"1.0.0\\\"\""
DEFINES += QTPLUS_LIBRARY

# Dependencies
INCLUDEPATH += $$PWD/../qt-plus/source/cpp

# Sources
include(CuteGit.pri)

# Functions
include($$PWD/../qt-plus/functions.pri)

# Directories
DESTDIR = $$OUT_PWD/bin

# Target binary name
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
# In order to activate deployment, add "deploy=1" to qmake arguments
QT_BASE_PATH = $$getQtPath()
QT_BIN_PATH = $$getQtBinPath()
QT_LIB_PATH = $$getQtLibPath()

!isEmpty(deploy) {
    message("Deployment files will be copied after linkage, from $${QT_BASE_PATH}.")

    QMAKE_POST_LINK += $$copyFilesWithPathToDir($$QT_LIB_PATH, $$QT_LIB_NAMES, $$DESTDIR)
    QMAKE_POST_LINK += $$copyFilesWithPathToDir($$QT_BASE_PATH, $$QT_PLUGIN_NAMES, $$DESTDIR)
    QMAKE_POST_LINK += $$copyFilesToDir($$PWD, $$DEPLOY_NAMES, $$DESTDIR)
    QMAKE_POST_LINK += $$copyDirsToDir($$QT_BASE_PATH, $$QT_QML_NAMES, $$DESTDIR)

    unix {
        QMAKE_POST_LINK += $$makeExecutable($$EXEC_NAMES, $$DESTDIR)
    }
}

# Installer - WIP
# In order to activate installer creation, add "installer=1" to qmake arguments
#!isEmpty(deploy) {
#    !isEmpty(installer) {
#        installerBin = $$QT_BIN_PATH/binarycreator.exe
#        packageDirectory = $$DESTDIR
#        configFile = $$PWD/deploy/installer-config.xml
#        cmd = $$installerBin --offline-only -p $$packageDirectory -c $$configFile $${TARGET}Installer
#    }
#}
