
// Qt
#include <QDebug>
#include <QApplication>
#include <QUrl>
#include <QMessageBox>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

/*!
    \class CController
    \inmodule CuteGit
    \section1 General
    This is the high level controller of the application.
    It also servers as the interface between UI and business logic.
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

    \code

                      /)-._
                     Y. ' _]
              ,.._   |`--"=
             /    "-/   \
    /)  sk  |   |_     `\|___
    \:::::::\___/_\__\_______\

    \endcode
*/

//-------------------------------------------------------------------------------------------------

const QString sParamConfiguration = "Configuration";
const QString sParamOpenRepositories = "OpenRepositories";
const QString sParamKnownRepositories = "KnownRepositories";
const QString sParamRepository = "Repository";
const QString sParamPath = "Path";
const QString sParamHistory = "History";
const QString sParamLastBrowsedRepositoryURL = "LastBrowsedRepositoryURL";
const QString sParamLastBrowsedRepositoryPath = "LastBrowsedRepositoryPath";

const QString CController::m_sSharedKey = "CuteGit-Shared-Memory";

//-------------------------------------------------------------------------------------------------

CController::CController(QObject* parent)
    : QObject(parent)
    , m_pCloneOutputModel(new QStringListModel(this))
    , m_pKnownRepositoryModel(new QStringListModel(this))
    , m_pOpenRepositoryModel(new CRepositoryModel(this))
    , m_pCurrentRepository(nullptr)
    , m_iCurrentRepositoryIndex(-1)
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
    , m_pCloneOutputModel(nullptr)
    , m_pKnownRepositoryModel(nullptr)
    , m_pOpenRepositoryModel(nullptr)
    , m_pCurrentRepository(nullptr)
    , m_iCurrentRepositoryIndex(-1)
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

    if (not m_tShared.detach())
    {
        qWarning() << "Could not detach from shared memory segment";
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setCurrentRepositoryIndex(int iValue)
{
    if (m_iCurrentRepositoryIndex != iValue)
    {
        m_iCurrentRepositoryIndex = iValue;
        emit currentRepositoryIndexChanged();

        if (m_iCurrentRepositoryIndex != -1 && m_pOpenRepositoryModel->rowCount() > 0)
        {
            CRepository* pRepository = m_pOpenRepositoryModel->repositories()[m_iCurrentRepositoryIndex];

            if (pRepository != nullptr)
                setCurrentRepository(pRepository);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowClean(bool bValue)
{
    if (m_bShowClean != bValue)
    {
        m_bShowClean = bValue;
        emit showCleanChanged();

        m_pOpenRepositoryModel->fileFiltersChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowAdded(bool bValue)
{
    if (m_bShowAdded != bValue)
    {
        m_bShowAdded = bValue;
        emit showAddedChanged();

        m_pOpenRepositoryModel->fileFiltersChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowModified(bool bValue)
{
    if (m_bShowModified != bValue)
    {
        m_bShowModified = bValue;
        emit showModifiedChanged();

        m_pOpenRepositoryModel->fileFiltersChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowDeleted(bool bValue)
{
    if (m_bShowDeleted != bValue)
    {
        m_bShowDeleted = bValue;
        emit showDeletedChanged();

        m_pOpenRepositoryModel->fileFiltersChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CController::setShowUntracked(bool bValue)
{
    if (m_bShowUntracked != bValue)
    {
        m_bShowUntracked = bValue;
        emit showUntrackedChanged();

        m_pOpenRepositoryModel->fileFiltersChanged();
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

    // Save known repository list
    CXMLNode xKnownRepositories(sParamKnownRepositories);
    QStringList lKnownRepositoryPaths = m_pKnownRepositoryModel->stringList();
    for (QString sPath : lKnownRepositoryPaths)
    {
        CXMLNode xRepository(sParamRepository);
        xRepository.attributes()[sParamPath] = sPath;
        xKnownRepositories << xRepository;
    }
    xConfig << xKnownRepositories;

    // Save open repository list
    CXMLNode xOpenRepositories(sParamOpenRepositories);
    for (CRepository* pRepository : m_pOpenRepositoryModel->repositories())
    {
        CXMLNode xOpenRepository(sParamRepository);
        xOpenRepository.attributes()[sParamPath] = pRepository->repositoryPath();
        xOpenRepositories << xOpenRepository;
    }
    xConfig << xOpenRepositories;

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

    // Load known repository list
    CXMLNode xKnownRepositories = xConfig.getNodeByTagName(sParamKnownRepositories);
    QVector<CXMLNode> xKnownRepositoryList = xKnownRepositories.getNodesByTagName(sParamRepository);

    QStringList lKnownRepositoryPaths;
    for (CXMLNode xRepository : xKnownRepositoryList)
    {
        lKnownRepositoryPaths << xRepository.attributes()[sParamPath];
    }
    m_pKnownRepositoryModel->setStringList(lKnownRepositoryPaths);

    // Load open repositories
    CXMLNode xOpenRepositories = xConfig.getNodeByTagName(sParamOpenRepositories);
    QVector<CXMLNode> xOpenRepositoryList = xOpenRepositories.getNodesByTagName(sParamRepository);
    for (CXMLNode xOpenRepository : xOpenRepositoryList)
    {
        QString sPath = xOpenRepository.attributes()[sParamPath];
        openRepository(sPath);
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

void CController::cloneRepository(QString sRepositoryURL, QString sRepositoryPath)
{
    if (sRepositoryPath.startsWith("file:"))
        sRepositoryPath = QUrl(sRepositoryPath).toLocalFile();

    CEnums::ERepositoryType eType = CRepository::getRepositoryTypeFromURL(sRepositoryURL);

    if (eType != CEnums::UnknownRepositoryType)
    {
        m_sCloneCommandsRepositoryPath = sRepositoryPath;
        m_pCloneCommands = CRepository::getCommandsForRepositoryType(eType);

        connect(m_pCloneCommands, &CCommands::newOutputString, this, &CController::onNewCloneOutput);

        m_pCloneCommands->cloneRepository(sRepositoryURL, sRepositoryPath);
    }
}

//-------------------------------------------------------------------------------------------------

void CController::openRepository(QString sRepositoryPath)
{
    if (sRepositoryPath.startsWith("file:"))
        sRepositoryPath = QUrl(sRepositoryPath).toLocalFile();

    if (m_pOpenRepositoryModel->hasRepository(sRepositoryPath))
        return;

    if (not sRepositoryPath.isEmpty())
    {
        if (QDir(sRepositoryPath).exists())
        {
            CEnums::ERepositoryType eType = CRepository::getRepositoryTypeFromFolder(sRepositoryPath);

            // If specified directory is a repository
            if (eType != CEnums::UnknownRepositoryType)
            {
                // Create a new repository item
                CRepository* pRepository = new CRepository(sRepositoryPath, this, this);

                m_pOpenRepositoryModel->addRepository(pRepository);

                // Add this path to known repository model
                QStringList lRepositoryPaths = m_pKnownRepositoryModel->stringList();
                if (not lRepositoryPaths.contains(sRepositoryPath))
                    lRepositoryPaths << sRepositoryPath;
                m_pKnownRepositoryModel->setStringList(lRepositoryPaths);

                setStatusText(QString(tr("%1 opened.")).arg(pRepository->repositoryName()));
            }
            else
            {
                setStatusText(QString(tr("%1 is not a repository.\nPlease select a folder containing a repository.")).arg(sRepositoryPath));
            }
        }
        else
        {
            // Clears this path from known repository model
            QStringList lKnownRepositoryPaths = m_pKnownRepositoryModel->stringList();
            lKnownRepositoryPaths.removeAll(sRepositoryPath);
            m_pKnownRepositoryModel->setStringList(lKnownRepositoryPaths);

            setStatusText(QString(tr("%1 does not exist. Ignoring.")).arg(sRepositoryPath));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CController::removeRepository(int iRepositoryIndex)
{
    m_pOpenRepositoryModel->removeRepository(iRepositoryIndex);
}

//-------------------------------------------------------------------------------------------------

QString CController::repositoryNameFromPath(const QString& sPath)
{
    return sPath.split(PATH_SEP).last();
}

//-------------------------------------------------------------------------------------------------

void CController::onNewCloneOutput(CEnums::EProcessCommand eCommand, QString sOutput)
{
    Q_UNUSED(eCommand);
    Q_UNUSED(sOutput);

    switch (eCommand)
    {
    default:
    case CEnums::eCloneRepository:
    {
        setStatusText(sOutput.split(NEW_LINE).last());
        break;
    }

    case CEnums::eCloneRepositoryFinished:
    {
        if (m_pCloneCommands != nullptr)
            delete m_pCloneCommands;

        m_pCloneCommands = nullptr;

        // Open the cloned repository
        if (true)
        {
            openRepository(m_sCloneCommandsRepositoryPath);
        }

        break;
    }
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

            if (m_pCurrentRepository != nullptr && m_pCurrentRepository->commands() != nullptr)
                m_pCurrentRepository->commands()->editSequenceFile(sFileName);

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
