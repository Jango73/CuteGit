
// Qt
#include <QDebug>
#include <QQuickStyle>

// qt-plus
#include <CXMLNode.h>

// Application
#include "CuteGit.h"
#include "CFileModel.h"
#include "CFileModelProxy.h"

//-------------------------------------------------------------------------------------------------

CuteGit::CuteGit(bool bMasterMode)
    : m_pController(nullptr)
    , m_pEngine(nullptr)
{
    if (bMasterMode)
    {
        // Register types
        // qmlRegisterType<CUINotification>("CuteGit", 1, 0, "SomeClass");
        qRegisterMetaType<CXMLNode>();
        qRegisterMetaType<CProcessCommand::EProcessCommand>("CProcessCommand::EProcessCommand");
        qRegisterMetaType<CFileModel::ERepositoryStatus>("CFileModel::ERepositoryStatus");

        qmlRegisterUncreatableType<CFileModel>("CuteGit", 1, 0, "CFileModel", "Cannot create a FileSystemModel instance.");
        qmlRegisterUncreatableType<CFileModelProxy>("CuteGit", 1, 0, "CFileModelProxy", "Cannot create a FileSystemModelProxy instance.");

        // Create controller and QML engine
        m_pController = new CController();
        m_pEngine = new QQmlApplicationEngine();

        // Set context properties
        m_pEngine->rootContext()->setContextProperty("controller", m_pController);

        QQuickStyle::setStyle("Material");

        // Load UI
        m_pEngine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    }
    else
    {
        m_pController = new CController();
    }
}

//-------------------------------------------------------------------------------------------------

CuteGit::~CuteGit()
{
    delete m_pEngine;
    delete m_pController;
}
