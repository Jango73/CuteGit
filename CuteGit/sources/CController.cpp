
// Qt
#include <QDebug>
#include <QUrl>
#include <QApplication>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

const QString sParamConfiguration = "Configuration";
const QString sParamCurrentRepository = "CurrentRepository";
const QString sParamRepositories = "Repositories";
const QString sParamRepository = "Repository";
const QString sParamPath = "Path";

const QString CController::m_sSharedKey = "CuteGit-Shared-Memory";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject* parent)
    : QObject(parent)
    , m_pCommands(new CGitCommands())
    , m_pFileModel(nullptr)
    , m_pFileModelProxy(new CFileModelProxy(this))
    , m_pRepositoryModel(new QStringListModel(this))
    , m_pCommandOutputModel(new QStringListModel(this))
    , m_bMasterMode(true)
    , m_tShared(m_sSharedKey, this)
    , m_tSharedTimer(this)
{
    loadConfiguration();

    if (m_tShared.create(SHARED_MEMORY_MAX))
    {
        clearSharedMemory();

        connect(&m_tSharedTimer, &QTimer::timeout, this, &CController::onSharedTimerTick);
        m_tSharedTimer.start(500);
    }
    else
    {
        qWarning() << "Could not create shared memory segment";
    }
}

//-------------------------------------------------------------------------------------------------

CController::CController(bool bDummy, QObject* parent)
    : QObject(parent)
    , m_pCommands(new CGitCommands())
    , m_pFileModel(nullptr)
    , m_pFileModelProxy(nullptr)
    , m_pRepositoryModel(nullptr)
    , m_pCommandOutputModel(nullptr)
    , m_bMasterMode(false)
    , m_tShared(m_sSharedKey, this)
    , m_tSharedTimer(this)
{
    Q_UNUSED(bDummy);

    if (m_tShared.attach())
    {
        connect(&m_tSharedTimer, &QTimer::timeout, this, &CController::onSharedTimerTick);
        m_tSharedTimer.start(500);

        setSharedOperation(eSOSlaveRequestEdit);
    }
    else
    {
        qWarning() << "Could not attach to shared memory segment";
    }
}

//-------------------------------------------------------------------------------------------------

CController::~CController()
{
    if (m_bMasterMode)
        saveConfiguration();

    if (m_tShared.detach() == false)
    {
        qWarning() << "Could not detach from shared memory segment";
    }
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

            // Clear command output
            m_pCommandOutputModel->setStringList(QStringList());
        }
        else
        {
            onNewOutput(QString(tr("%1 is not a GIT repository.\nPlease select a folder containing a GIT repository.")).arg(sPath));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setSharedOperation(ESharedOperation eOperation)
{
    if (m_tShared.lock())
    {
        SMemoryStruct* pData = static_cast<SMemoryStruct*>(m_tShared.data());
        pData->eOperation = eOperation;
        m_tShared.unlock();
    }
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

CController::ESharedOperation CController::sharedOperation()
{
    ESharedOperation eReturnValue = eSONone;
    if (m_tShared.lock())
    {
        SMemoryStruct* pData = static_cast<SMemoryStruct*>(m_tShared.data());
        eReturnValue = pData->eOperation;
        m_tShared.unlock();
    }
    return eReturnValue;
}

//-------------------------------------------------------------------------------------------------

void CController::saveConfiguration()
{
    CXMLNode xConfig(sParamConfiguration);

    CXMLNode xCurrentRepository(sParamCurrentRepository);
    xCurrentRepository.attributes()[sParamPath] = m_pFileModel->rootPath();
    xConfig << xCurrentRepository;

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

    CXMLNode xCurrentRepository = xConfig.getNodeByTagName(sParamCurrentRepository);
    QString sCurrentPath = xCurrentRepository.attributes()[sParamPath];

    CXMLNode xRepositories = xConfig.getNodeByTagName(sParamRepositories);
    QVector<CXMLNode> xRepositoryList = xRepositories.getNodesByTagName(sParamRepository);

    QStringList lRepositoryPaths;

    for (CXMLNode xRepository : xRepositoryList)
    {
        lRepositoryPaths << xRepository.attributes()[sParamPath];
    }

    m_pRepositoryModel->setStringList(lRepositoryPaths);

    if (sCurrentPath.isEmpty() == false)
    {
        setRepositoryPath(sCurrentPath);
    }
    else if (lRepositoryPaths.count() > 0)
    {
        setRepositoryPath(lRepositoryPaths[0]);
    }
}

//-------------------------------------------------------------------------------------------------

void CController::clearSharedMemory()
{
    if (m_tShared.lock())
    {
        SMemoryStruct* pData = static_cast<SMemoryStruct*>(m_tShared.data());
        memset(pData, 0, sizeof(SMemoryStruct));
        m_tShared.unlock();
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
        sLine = sLine.trimmed();

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

//-------------------------------------------------------------------------------------------------

void CController::onSharedTimerTick()
{
    if (m_bMasterMode)
    {
        if (sharedOperation() == eSOSlaveRequestEdit)
        {
            qDebug() << "Master sees eSOSlaveRequestEdit";

            setSharedOperation(eSONone);
        }
    }
    else
    {
        if (sharedOperation() == eSOMasterFinishedEdit)
        {
            qDebug() << "Slave sees eSOMasterFinishedEdit";

            setSharedOperation(eSONone);

            quit();
        }
    }
}
