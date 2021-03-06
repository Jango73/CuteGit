
HEADERS += \
    ../qt-plus/source/cpp/CXMLNode.h \
    sources/CBranch.h \
    sources/CBranchModel.h \
    sources/CController.h \
    sources/CDiffLine.h \
    sources/CDiffModel.h \
    sources/CDiffModelProxy.h \
    sources/CEnums.h \
    sources/CFlatFileModel.h \
    sources/CFlatFileModelProxy.h \
    sources/CGraphLine.h \
    sources/CGraphModel.h \
    sources/CLabel.h \
    sources/CLabelModel.h \
    sources/CLogLine.h \
    sources/CLogLineCollection.h \
    sources/CLogModel.h \
    sources/CLogModelProxy.h \
    sources/CRepoFile.h \
    sources/CRepository.h \
    sources/CRepositoryModel.h \
    sources/CStagedFileModelProxy.h \
    sources/CTreeFileModel.h \
    sources/CTreeFileModelProxy.h \
    sources/CUtils.h \
    sources/CuteGit.h \
    sources/Strings.h \
    sources/commands/CCommands.h \
    sources/commands/CExecution.h \
    sources/commands/CGerritCommands.h \
    sources/commands/CGitCommands.h \
    sources/commands/CHgCommands.h \
    sources/commands/CSvnCommands.h \

SOURCES += \
    ../qt-plus/source/cpp/CXMLNode.cpp \
    sources/CBranchModel.cpp \
    sources/CController.cpp \
    sources/CDiffModel.cpp \
    sources/CDiffModelProxy.cpp \
    sources/CFlatFileModel.cpp \
    sources/CFlatFileModelProxy.cpp \
    sources/CGraphModel.cpp \
    sources/CLabelModel.cpp \
    sources/CLogLineCollection.cpp \
    sources/CLogModel.cpp \
    sources/CLogModelProxy.cpp \
    sources/CRepoFile.cpp \
    sources/CRepository.cpp \
    sources/CRepositoryModel.cpp \
    sources/CStagedFileModelProxy.cpp \
    sources/CTreeFileModel.cpp \
    sources/CTreeFileModelProxy.cpp \
    sources/CUtils.cpp \
    sources/CuteGit.cpp \
    sources/Strings.cpp \
    sources/commands/CCommands.cpp \
    sources/commands/CExecution.cpp \
    sources/commands/CGerritCommands.cpp \
    sources/commands/CGitCommands.cpp \
    sources/commands/CHgCommands.cpp \
    sources/commands/CSvnCommands.cpp \
    sources/main.cpp \

RESOURCES += \
    resources.qrc

TRANSLATIONS = \
    i18n/CuteGit_fr.ts \
    i18n/CuteGit_de.ts \
    i18n/CuteGit_es.ts \
    i18n/CuteGit_ja.ts \
    i18n/CuteGit_zh.ts \

unix {
    QT_LIB_NAMES = \
        libicudata.so.56 \
        libicui18n.so.56 \
        libicuuc.so.56 \
        libQt5Core.so.5 \
        libQt5DBus.so.5 \
        libQt5Gui.so.5 \
        libQt5Network.so.5 \
        libQt5OpenGL.so.5 \
        libQt5Qml.so.5 \
        libQt5Quick.so.5 \
        libQt5QuickControls2.so.5 \
        libQt5QuickTemplates2.so.5 \
        libQt5QuickWidgets.so.5 \
        libQt5Svg.so.5 \
        libQt5Widgets.so.5 \
        libQt5XcbQpa.so.5 \
        libQt5Xml.so.5

    QT_PLUGIN_NAMES = \
        plugins/imageformats/libqico.so \
        plugins/imageformats/libqjpeg.so \
        plugins/imageformats/libqsvg.so \
        plugins/platforminputcontexts/libcomposeplatforminputcontextplugin.so \
        plugins/platforms/libqxcb.so \
        plugins/xcbglintegrations/libqxcb-egl-integration.so \
        plugins/xcbglintegrations/libqxcb-glx-integration.so

    DEPLOY_NAMES = \
        deploy/linux/qt.conf \
        deploy/linux/CuteGit.sh

    EXEC_NAMES = \
        CuteGit.sh
} else {
    QT_LIB_NAMES = \
        libgcc_s_seh-1.dll \
#        libstdc++-6.dll \
        libwinpthread-1.dll \
        Qt5Core.dll \
        Qt5DBus.dll \
        Qt5Gui.dll \
        Qt5Network.dll \
        Qt5OpenGL.dll \
        Qt5Qml.dll \
        Qt5Quick.dll \
        Qt5QuickControls2.dll \
        Qt5QuickTemplates2.dll \
        Qt5QuickWidgets.dll \
        Qt5Svg.dll \
        Qt5Widgets.dll \
        Qt5Xml.dll

    QT_PLUGIN_NAMES = \
        plugins/imageformats/qico.dll \
        plugins/imageformats/qjpeg.dll \
        plugins/imageformats/qsvg.dll \
        plugins/platforms/qwindows.dll

    DEPLOY_NAMES = \
        deploy/linux/qt.conf

    EXEC_NAMES =
}

QT_QML_NAMES = \
    qml/Qt/labs/folderlistmodel \
    qml/Qt/labs/settings \
    qml/Qt/labs/platform \
    qml/QtQml/Models.2 \
    qml/QtQuick/Controls \
    qml/QtQuick/Controls.2 \
    qml/QtQuick/Dialogs \
    qml/QtQuick/Layouts \
    qml/QtQuick/PrivateWidgets \
    qml/QtQuick/Scene2D \
    qml/QtQuick/Templates.2 \
    qml/QtQuick/Window.2 \
    qml/QtQuick/XmlListModel \
    qml/QtQuick.2
