
HEADERS += \
    ../qt-plus/source/cpp/CXMLNode.h \
    sources/CEnums.h \
    sources/CUtils.h \
    sources/CBranch.h \
    sources/CExecution.h \
    sources/CCommands.h \
    sources/CGitCommands.h \
    sources/CGerritCommands.h \
    sources/CSvnCommands.h \
    sources/CHgCommands.h \
    sources/CBranchModel.h \
    sources/CTreeFileModel.h \
    sources/CTreeFileModelProxy.h \
    sources/CFlatFileModel.h \
    sources/CFlatFileModelProxy.h \
    sources/CStagedFileModelProxy.h \
    sources/CRepoFile.h \
    sources/CLabel.h \
    sources/CLabelModel.h \
    sources/CLogModel.h \
    sources/CLogModelProxy.h \
    sources/CLogLine.h \
    sources/CLogLineCollection.h \
    sources/CGraphModel.h \
    sources/CGraphLine.h \
    sources/CDiffModel.h \
    sources/CDiffModelProxy.h \
    sources/CDiffLine.h \
    sources/CRepository.h \
    sources/CRepositoryModel.h \
    sources/CController.h \
    sources/CuteGit.h \

SOURCES += \
    ../qt-plus/source/cpp/CXMLNode.cpp \
    sources/CUtils.cpp \
    sources/CExecution.cpp \
    sources/CCommands.cpp \
    sources/CGitCommands.cpp \
    sources/CGerritCommands.cpp \
    sources/CSvnCommands.cpp \
    sources/CHgCommands.cpp \
    sources/CBranchModel.cpp \
    sources/CTreeFileModel.cpp \
    sources/CTreeFileModelProxy.cpp \
    sources/CFlatFileModel.cpp \
    sources/CFlatFileModelProxy.cpp \
    sources/CStagedFileModelProxy.cpp \
    sources/CRepoFile.cpp \
    sources/CLabelModel.cpp \
    sources/CLogModel.cpp \
    sources/CLogModelProxy.cpp \
    sources/CLogLineCollection.cpp \
    sources/CGraphModel.cpp \
    sources/CDiffModel.cpp \
    sources/CDiffModelProxy.cpp \
    sources/CRepository.cpp \
    sources/CRepositoryModel.cpp \
    sources/CController.cpp \
    sources/CuteGit.cpp \
    sources/main.cpp \

RESOURCES += \
    resources.qrc

TRANSLATIONS = \
    i18n/CuteGit_fr.ts \
    i18n/CuteGit_de.ts

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
