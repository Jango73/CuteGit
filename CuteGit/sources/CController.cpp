
// Qt
#include <QDebug>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

const QString sParamRepositories = "Repositories";
const QString sParamRepository = "Repository";
const QString sParamPath = "Path";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pCommands(new CGitCommands())
    , m_pFileModel(new CFileModel(this, this))
    , m_pRepositoryModel(new QStringListModel())
{
    loadConfiguration();
}

//-------------------------------------------------------------------------------------------------

CController::~CController()
{
    saveConfiguration();
}

//-------------------------------------------------------------------------------------------------

QString CController::repositoryPath() const
{
    return m_pFileModel->rootPath();
}

//-------------------------------------------------------------------------------------------------

void CController::saveConfiguration()
{
}

//-------------------------------------------------------------------------------------------------

void CController::loadConfiguration()
{
    CXMLNode xConfig = CXMLNode::load(CONFIG_FILE_NAME);

    CXMLNode xRepositories = xConfig.getNodeByTagName(sParamRepositories);
    QVector<CXMLNode> xRepositoryList = xRepositories.getNodesByTagName(sParamRepository);

    QStringList lRepositoryPaths;

    for (CXMLNode xRepository : xRepositoryList)
    {
        lRepositoryPaths << xRepository.attributes()[sParamPath];
    }

    m_pRepositoryModel->setStringList(lRepositoryPaths);

    if (lRepositoryPaths.count() > 0)
        m_pFileModel->setRootPath(lRepositoryPaths[0]);
}

//-------------------------------------------------------------------------------------------------

void CController::setRepository(QString sPath)
{
    if (m_pFileModel->rootPath() != sPath)
    {
        m_pFileModel->setRootPath(sPath);
        emit repositoryPathChanged();
    }
}
