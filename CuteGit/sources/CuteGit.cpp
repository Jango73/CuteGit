
// Qt
#include <QDebug>
#include <QQuickStyle>

// qt-plus
#include <CXMLNode.h>

// Application
#include "CuteGit.h"
#include "CRepository.h"
#include "CTreeFileModel.h"
#include "CTreeFileModelProxy.h"
#include "CDiffLine.h"

//-------------------------------------------------------------------------------------------------

CuteGit::CuteGit(bool bMasterMode, const QString& sSequenceFileName)
    : m_pController(nullptr)
    , m_pEngine(nullptr)
{
    if (bMasterMode)
    {
        // Register types
        // qmlRegisterType<CUINotification>("CuteGit", 1, 0, "SomeClass");
        qRegisterMetaType<CXMLNode>();
        qRegisterMetaType<CProcessCommand::EProcessCommand>("CProcessCommand::EProcessCommand");
        qRegisterMetaType<CRepository::ERepositoryStatus>("CRepository::ERepositoryStatus");
        qRegisterMetaType<CDiffLine::EDiffOperation>("CDiffLine::EDiffOperation");

        qmlRegisterUncreatableType<CTreeFileModel>("CuteGit", 1, 0, "CTreeFileModel", "Cannot create a FileSystemModel instance.");
        qmlRegisterUncreatableType<CTreeFileModelProxy>("CuteGit", 1, 0, "CFileModelProxy", "Cannot create a FileSystemModelProxy instance.");
        qmlRegisterUncreatableType<CDiffLine>("CuteGit", 1, 0, "CDiffLine", "Cannot create a CDiffLine instance.");

        // Create controller and QML engine
        m_pController = new CController(this);
        m_pEngine = new QQmlApplicationEngine(this);

        // Set context properties
        m_pEngine->rootContext()->setContextProperty("controller", m_pController);

        QQuickStyle::setStyle("Material");

        // Load UI
        m_pEngine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    }
    else
    {
        m_pController = new CController(sSequenceFileName, this);
    }
}

//-------------------------------------------------------------------------------------------------

CuteGit::~CuteGit()
{
    delete m_pEngine;
    delete m_pController;
}
