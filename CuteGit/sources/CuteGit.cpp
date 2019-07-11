
// Qt
#include <QDebug>

// Application
#include "CuteGit.h"
#include "CFileModel.h"

//-------------------------------------------------------------------------------------------------

CuteGit::CuteGit()
    : m_pController(new CController())
    , m_pEngine(new QQmlApplicationEngine())
{
    // Register types
    // qmlRegisterType<CUINotification>("CuteGit", 1, 0, "SomeClass");
    // qRegisterMetaType<CXMLNode>();

    qmlRegisterUncreatableType<CFileModel>("io.qt.examples.quick.controls.filesystembrowser", 1, 0,
                                                       "FileSystemModel", "Cannot create a FileSystemModel instance.");

    // Update context
    m_pEngine->rootContext()->setContextProperty("controller", m_pController);
    m_pEngine->rootContext()->setContextProperty("fileSystemModel", m_pController->fileModel());
    m_pEngine->rootContext()->setContextProperty("rootPathIndex", m_pController->fileModel()->index(m_pController->fileModel()->rootPath()));

    // Load UI
    m_pEngine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
}

//-------------------------------------------------------------------------------------------------

CuteGit::~CuteGit()
{
    delete m_pController;
    delete m_pEngine;
}
