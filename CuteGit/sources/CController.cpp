
// Qt
#include <QDebug>
#include <QUrl>
#include <QApplication>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

const QString sParamConfiguration = "Configuration";
const QString sParamRepositories = "Repositories";
const QString sParamRepository = "Repository";
const QString sParamPath = "Path";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pCommands(new CGitCommands())
    , m_pFileModel(nullptr)
    , m_pFileModelProxy(new CFileModelProxy(this))
    , m_pRepositoryModel(new QStringListModel(this))
    , m_pCommandOutputModel(new QStringListModel(this))
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
    if (m_pFileModel != nullptr)
    {
        return m_pFileModel->rootPath();
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

void CController::setRepositoryPath(QString sPath)
{
    if (sPath.startsWith("file:"))
        sPath = QUrl(sPath).toLocalFile();

    if (m_pFileModel == nullptr || sPath != m_pFileModel->rootPath())
    {
        if (QDir(QString("%1/.git").arg(sPath)).exists())
        {
            if (m_pFileModel != nullptr)
            {
                delete m_pFileModel;
            }

            m_pFileModel = new CFileModel(this, this);
            m_pFileModel->setRootPath(sPath);
            m_pFileModelProxy->setSourceModel(m_pFileModel);

            emit repositoryPathChanged();
            emit fileModelChanged();
            emit fileModelProxyChanged();

            connect(m_pFileModel, &CFileModel::newOutput, this, &CController::onNewOutput);

            // Add repository to model
            QStringList lRepositoryPaths = m_pRepositoryModel->stringList();

            if (lRepositoryPaths.contains(sPath) == false)
                lRepositoryPaths << sPath;

            m_pRepositoryModel->setStringList(lRepositoryPaths);
        }
        else
        {
            onNewOutput(QString(tr("%1 is not a GIT repository.\nPlease select a folder containing a GIT repository.")).arg(sPath));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CController::saveConfiguration()
{
    CXMLNode xConfig(sParamConfiguration);

    CXMLNode xRepositories(sParamRepositories);

    // Add repository to model
    QStringList lRepositoryPaths = m_pRepositoryModel->stringList();

    for (QString sPath : lRepositoryPaths)
    {
        CXMLNode xRepository(sParamRepository);
        xRepository.attributes()[sParamPath] = sPath;
        xRepositories << xRepository;
    }

    xConfig << xRepositories;

    xConfig.saveXMLToFile(CONFIG_FILE_NAME);
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
    {
        setRepositoryPath(lRepositoryPaths[0]);
    }
}

//-------------------------------------------------------------------------------------------------

void CController::quit()
{
    QApplication::quit();
}

//-------------------------------------------------------------------------------------------------

void CController::clearOutput()
{
    m_pCommandOutputModel->setStringList(QStringList());
}

//-------------------------------------------------------------------------------------------------

void CController::onNewOutput(QString sOutput)
{
    QStringList lNewList = sOutput.split("\n");
    QStringList lData = m_pCommandOutputModel->stringList();
    bool bHasNewLine = false;

    for (QString sLine : lNewList)
    {
        if (sLine.isEmpty() == false)
        {
            bHasNewLine = true;
            lData << sLine;
        }
    }

    if (bHasNewLine)
    {
        lData << "----------------------------------------------------------------------------------------------------";
    }

    while (lData.count() > 50)
        lData.removeFirst();

    m_pCommandOutputModel->setStringList(lData);
}
