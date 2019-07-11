
// Qt
#include <QDebug>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

const QString sParamRepositories = "Repositories";
const QString sParamRepository = "Repository";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pFileModel(new CFileModel(this, this))
    , m_pCommands(new CGitCommands())
{
    loadRepositories();
}

//-------------------------------------------------------------------------------------------------

CController::~CController()
{
}

//-------------------------------------------------------------------------------------------------

void CController::loadRepositories()
{
    CXMLNode xConfig = CXMLNode::load(CONFIG_FILE_NAME);

    CXMLNode xRepositories = xConfig.getNodeByTagName(sParamRepositories);
    QVector<CXMLNode> xRepositoryList = xRepositories.getNodesByTagName(sParamRepository);

    for (CXMLNode xRepository : xRepositoryList)
    {

    }

    m_pFileModel->setRootPath("D:/Work/Projects/various/AGL32");
}
