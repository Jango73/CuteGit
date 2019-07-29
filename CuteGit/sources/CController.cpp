
// Qt
#include <QDebug>
#include <QApplication>
#include <QUrl>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

/*!
    \class CController
    \inmodule CuteGit
    \section1 The workflow of Git interactive rebase
    * CuteGit-1-A runs (1 is first instance, A is main thread)
    * CuteGit-1-A calls CCommands::exec with "git rebase --interactive" and "GIT_SEQUENCE_EDITOR=<path-to-cutegit>"
    * CuteGit-1-B launches process (1 is first instance, B is process execution thread)
    * Git starts
    * Git launches process : <path-to-cutegit> <path-to-sequence-file>
    * CuteGit-2 starts in slave mode (because it has one argument)
    * CuteGit-2-A asks (via shared memory) for sequence file edition
    * CuteGit-1-A edits the sequence file for slave and notifies it
    * CuteGit-1-B is still waiting for Git process to end
    * CuteGit-2 quits
    * If changing a commit:
    ** Git launches process : <path-to-cutegit> <path-to-commit-file>
    ** CuteGit-2 starts in slave mode (because it has one argument)
    ** CuteGit-2-A asks (via shared memory) for commit file edition
    ** CuteGit-1-A edits the sequence file for slave and notifies it
    ** CuteGit-2 quits
    * Git quits
    * CuteGit-1-B returns

    (\../)
    ( *.*)
    (")_(")
*/

//-------------------------------------------------------------------------------------------------

const QString sParamConfiguration = "Configuration";
const QString sParamCurrentRepository = "CurrentRepository";
const QString sParamRepositories = "Repositories";
const QString sParamRepository = "Repository";
const QString sParamPath = "Path";
const QString sParamHistory = "History";
const QString sParamLastBrowsedRepositoryURL = "LastBrowsedRepositoryURL";
const QString sParamLastBrowsedRepositoryPath = "LastBrowsedRepositoryPath";

const QString CController::m_sSharedKey = "CuteGit-Shared-Memory";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject* parent)
    : QObject(parent)
    , m_pRepositoryModel(new QStringListModel(this))
    , m_pCommandOutputModel(new QStringListModel(this))
    , m_pRepository(new CRepository("", this, this))
    , m_bShowClean(false)
    , m_bShowAdded(true)
    , m_bShowModified(true)
    , m_bShowDeleted(true)
    , m_bShowUntracked(false)
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

CController::CController(QString sSequenceFileName, QObject* parent)
    : QObject(parent)
    , m_pRepositoryModel(nullptr)
    , m_pCommandOutputModel(nullptr)
    , m_pRepository(nullptr)
    , m_bShowClean(false)
    , m_bShowAdded(true)
    , m_bShowModified(true)
    , m_bShowDeleted(true)
    , m_bShowUntracked(false)
    , m_bMasterMode(false)
    , m_tShared(m_sSharedKey, this)
    , m_tSharedTimer(this)
{
    Q_UNUSED(sSequenceFileName);

    if (m_tShared.attach())
    {
        connect(&m_tSharedTimer, &QTimer::timeout, this, &CController::onSharedTimerTick);
        m_tSharedTimer.start(500);

        setSequenceFileName(sSequenceFileName);
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

void CController::setShowClean(bool bValue)
{
    if (m_bShowClean != bValue)
    {
        m_bShowClean = bValue;
        if (m_pRepository != nullptr)
        {
            m_pRepository->treeFileModelProxy()->filterChanged();
            m_pRepository->flatFileModelProxy()->filterChanged();
        }
        emit showCleanChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowAdded(bool bValue)
{
    if (m_bShowAdded != bValue)
    {
        m_bShowAdded = bValue;
        if (m_pRepository != nullptr)
        {
            m_pRepository->treeFileModelProxy()->filterChanged();
            m_pRepository->flatFileModelProxy()->filterChanged();
        }
        emit showAddedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowModified(bool bValue)
{
    if (m_bShowModified != bValue)
    {
        m_bShowModified = bValue;
        if (m_pRepository != nullptr)
        {
            m_pRepository->treeFileModelProxy()->filterChanged();
            m_pRepository->flatFileModelProxy()->filterChanged();
        }
        emit showModifiedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowDeleted(bool bValue)
{
    if (m_bShowDeleted != bValue)
    {
        m_bShowDeleted = bValue;
        if (m_pRepository != nullptr)
        {
            m_pRepository->treeFileModelProxy()->filterChanged();
            m_pRepository->flatFileModelProxy()->filterChanged();
        }
        emit showDeletedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowUntracked(bool bValue)
{
    if (m_bShowUntracked != bValue)
    {
        m_bShowUntracked = bValue;
        if (m_pRepository != nullptr)
        {
            m_pRepository->treeFileModelProxy()->filterChanged();
            m_pRepository->flatFileModelProxy()->filterChanged();
        }
        emit showUntrackedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setRepositoryPath(QString sPath)
{
    if (sPath.startsWith("file:"))
        sPath = QUrl(sPath).toLocalFile();

    if (QDir(sPath).exists())
    {
        CEnums::ERepositoryType eType = CRepository::getRepositoryTypeFromFolder(sPath);

        // If specified directory is a repository
        if (eType != CEnums::UnknownRepositoryType)
        {
            // Delete the current repository item
            if (m_pRepository != nullptr)
                delete m_pRepository;

            // Create a new repository item
            m_pRepository = new CRepository(sPath, this, this);

            // Add this path to repository model
            QStringList lRepositoryPaths = m_pRepositoryModel->stringList();

            if (lRepositoryPaths.contains(sPath) == false)
                lRepositoryPaths << sPath;

            m_pRepositoryModel->setStringList(lRepositoryPaths);

            // Clear command output
            m_pCommandOutputModel->setStringList(QStringList());

            connect(m_pRepository, &CRepository::newOutput, this, &CController::onNewOutput);

            emit repositoryChanged();
            emit repositoryPathChanged();

            onNewOutput(QString(tr("Now working on %1.")).arg(sPath));
        }
        else
        {
            onNewOutput(QString(tr("%1 is not a repository.\nPlease select a folder containing a repository.")).arg(sPath));
        }
    }
    else
    {
        // Clears this path from repository model
        QStringList lRepositoryPaths = m_pRepositoryModel->stringList();
        lRepositoryPaths.removeAll(sPath);
        m_pRepositoryModel->setStringList(lRepositoryPaths);

        onNewOutput(QString(tr("%1 does not exist. Ignoring.")).arg(sPath));
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

void CController::setSequenceFileName(const QString& sSequenceFileName)
{
    if (m_tShared.lock())
    {
        SMemoryStruct* pData = static_cast<SMemoryStruct*>(m_tShared.data());
        strcpy(pData->sSequenceFileName, sSequenceFileName.toUtf8().constData());
        m_tShared.unlock();
    }
}


//-------------------------------------------------------------------------------------------------

QString CController::repositoryPath() const
{
    if (m_pRepository != nullptr)
        return m_pRepository->repositoryPath();

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

QString CController::sequenceFileName()
{
    QString sReturnValue;

    if (m_tShared.lock())
    {
        SMemoryStruct* pData = static_cast<SMemoryStruct*>(m_tShared.data());
        sReturnValue = QString::fromUtf8(pData->sSequenceFileName);
        m_tShared.unlock();
    }

    return sReturnValue;
}

//-------------------------------------------------------------------------------------------------

void CController::saveConfiguration()
{
    CXMLNode xConfig(sParamConfiguration);

    // Save history items
    CXMLNode xHistory(sParamHistory);
    xHistory.attributes()[sParamLastBrowsedRepositoryURL] = m_sLastBrowsedRepositoryURL;
    xHistory.attributes()[sParamLastBrowsedRepositoryPath] = m_sLastBrowsedRepositoryPath;
    xConfig << xHistory;

    // Save current repository
    CXMLNode xCurrentRepository(sParamCurrentRepository);
    xCurrentRepository.attributes()[sParamPath] = repository()->repositoryPath();
    xConfig << xCurrentRepository;

    // Save known repository list
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

    // Load history items
    CXMLNode xHistory = xConfig.getNodeByTagName(sParamHistory);
    m_sLastBrowsedRepositoryURL = xHistory.attributes()[sParamLastBrowsedRepositoryURL];
    m_sLastBrowsedRepositoryPath = xHistory.attributes()[sParamLastBrowsedRepositoryPath];

    // Load current repository
    CXMLNode xCurrentRepository = xConfig.getNodeByTagName(sParamCurrentRepository);
    QString sCurrentPath = xCurrentRepository.attributes()[sParamPath];

    // Load known repository list
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
    else
    {
        setRepositoryPath("");
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

void CController::cloneRepository(const QString& sRepositoryURL, const QString& sRepositoryPath)
{
    CEnums::ERepositoryType eType = CRepository::getRepositoryTypeFromURL(sRepositoryURL);

    m_pCloneCommands = CRepository::getCommandsForRepositoryType(eType);

    connect(m_pCloneCommands, &CCommands::newOutputString, this, &CController::onNewCloneOutput);
    m_pCloneCommands->cloneRepository(sRepositoryURL, sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CController::onNewCloneOutput(CEnums::EProcessCommand eCommand, QString sOutput)
{
    Q_UNUSED(eCommand);

    if (m_pCloneCommands != nullptr)
        m_pCloneCommands->deleteLater();

    m_pCloneCommands = nullptr;

    onNewOutput(sOutput);
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

        while (lData.count() > 50)
            lData.removeFirst();

        m_pCommandOutputModel->setStringList(lData);
    }
}

//-------------------------------------------------------------------------------------------------

void CController::onSharedTimerTick()
{
    if (m_bMasterMode)
    {
        if (sharedOperation() == eSOSlaveRequestEdit)
        {
            QString sFileName = sequenceFileName();

            if (m_pRepository != nullptr && m_pRepository->commands() != nullptr)
                m_pRepository->commands()->editSequenceFile(sFileName);

            setSharedOperation(eSOMasterFinishedEdit);
        }
    }
    else
    {
        if (sharedOperation() == eSOMasterFinishedEdit)
        {
            setSharedOperation(eSONone);

            quit();
        }
    }
}
