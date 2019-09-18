
// Qt
#include <QDebug>
#include <QRegExp>
#include <QDir>

// Application
#include "CHgCommands.h"

/*!
    \class CHgCommands
    \inmodule CuteGit
    \section1 General
    This is the repository control interface for Mercurial.

    \code

    /  \.-"""-./  \
    \    -   -    /
     |   o   o   |
     \  .-'''-.  /
      '-\__Y__/-'
         `---`

    \endcode
*/

//-------------------------------------------------------------------------------------------------
// Constant numbers

static int iLogFormatValueCount = 4;

//-------------------------------------------------------------------------------------------------
// Command strings

static const char* sCommandAdd                = "hg add \"%1\"";
static const char* sCommandAmend              = "hg commit --amend --date now";
static const char* sCommandBranchLog          = "hg log --template '{node} &&& {desc} &&& {author} &&& {date}\\n'";
static const char* sCommandCommit             = "hg commit -m \"%1\"";
static const char* sCommandFileLog            = "hg log --template '{node} &&& {desc} &&& {author} &&& {date}\\n' \"%1\"";
// static const char* sCommandGraph  = "hg log -G";
static const char* sCommandPull               = "hg pull";
static const char* sCommandPush               = "hg push";
static const char* sCommandRevert             = "hg revert \"%1\"";
static const char* sCommandStatus             = "hg status";
static const char* sCommandStatusForFile      = "hg status \"%1\"";

//-------------------------------------------------------------------------------------------------
// Regular expressions

static const char* sStatusRegExp = "([a-zA-Z?!])\\s(.*)";

//-------------------------------------------------------------------------------------------------
// Other strings

static const char* sLogFormatSplitter   = "&&&";

//-------------------------------------------------------------------------------------------------
// Status characters

const QString sStatusClean       = "C";
const QString sStatusAdded       = "A";
const QString sStatusModified    = "M";
const QString sStatusDeleted     = "R";
const QString sStatusUntracked   = "?";
const QString sStatusMissing     = "!";
const QString sStatusIgnored     = "I";
const QString sStatusOriginOfAdd = " ";

//-------------------------------------------------------------------------------------------------

CHgCommands::CHgCommands(CController* pController)
    : CCommands(pController)
{
    connect(this, &CCommands::execFinished, this, &CHgCommands::onExecFinished);
}

//-------------------------------------------------------------------------------------------------

CHgCommands::~CHgCommands()
{
}

//-------------------------------------------------------------------------------------------------

bool CHgCommands::can(CEnums::ECapability eWhat) const
{
    switch (eWhat)
    {
    case CEnums::Clone:
    case CEnums::ShowGlobalGraph:
    case CEnums::ShowBranchLog:
    case CEnums::ShowFileLog:
    case CEnums::Commit:
    case CEnums::Pull:
    case CEnums::BranchFromCommit:
        return true;

    default:
        return false;
    }
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::allFileStatus(const QString& sPath)
{
    CCleanFileLister* pLister = new CCleanFileLister();

    pLister->m_eCommand = CEnums::eAllFileStatus;
    pLister->m_sRootPath = sPath;
    pLister->setAutoDelete(true);

    connect(pLister, &CCleanFileLister::newOutputListOfCRepoFile, this, &CHgCommands::onNewOutputListOfCRepoFile);

    m_tPool.start(pLister);
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::changedFileStatus(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eChangedFileStatus, sPath, sCommandStatus));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::branchLog(const QString& sPath, int iFrom, int iCount)
{
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);

    QString sCommand = QString(sCommandBranchLog);
    exec(new CProcessCommand(CEnums::eBranchLog, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::fileLog(const QString& sPath, const QString& sFullName, int iFrom, int iCount)
{
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);

    QString sCommand = QString(sCommandFileLog).arg(sFullName);
    exec(new CProcessCommand(CEnums::eFileLog, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::toggleStaged(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandStatusForFile).arg(sFullName);
    QString sLine = execNow(sPath, sCommand);

    CRepoFile* pFile = repoFileForLine(sPath, sLine);

    if (pFile != nullptr)
    {
        stageFile(sPath, sFullName, not pFile->staged());
        delete pFile;
    }
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandAdd : sCommandRevert).arg(sFullName);
    exec(new CProcessCommand(CEnums::eStageFile, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::commit(const QString& sPath, const QString& sMessage)
{
    emit newOutputString(CEnums::eNotification, tr("Commiting..."));
    QString sCommand = QString(sCommandCommit).arg(sMessage);
    exec(new CProcessCommand(CEnums::eCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::amend(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Amending..."));
    QString sCommand = QString(sCommandAmend);
    exec(new CProcessCommand(CEnums::eAmend, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::push(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Pushing..."));
    QString sCommand = QString(sCommandPush);
    exec(new CProcessCommand(CEnums::ePush, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::pull(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Pulling..."));
    QString sCommand = QString(sCommandPull);
    exec(new CProcessCommand(CEnums::ePull, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CHgCommands::repoFileForLine(const QString &sPath, QString sLine)
{
    QRegExp tRegExp(sStatusRegExp);

#ifdef HAVE_QSTRING_BACK
    if (not sLine.isEmpty() && sLine.back() == '/')
        sLine.chop(1);
#endif

    if (tRegExp.indexIn(sLine) != -1)
    {
        QString sStatus = tRegExp.cap(1).trimmed();
        QString sRelativeName = tRegExp.cap(2).trimmed();
        QString sFullName = sPath + PATH_SEP + sRelativeName;
        QString sFileName = QFileInfo(sFullName).fileName();
        bool bStaged = true;

        CEnums::ERepoFileStatus eStatus = CEnums::eClean;

        if (not sStatus.isEmpty())
        {
            if (sStatus == sStatusClean)
                eStatus = CEnums::eClean;
            else if (sStatus == sStatusAdded)
                eStatus = CEnums::eAdded;
            else if (sStatus == sStatusModified)
                eStatus = CEnums::eModified;
            else if (sStatus == sStatusDeleted)
                eStatus = CEnums::eDeleted;
            else if (sStatus == sStatusIgnored)
                eStatus = CEnums::eIgnored;
            else if (sStatus == sStatusMissing)
                eStatus = CEnums::eMissing;
            else if (sStatus == sStatusUntracked)
            {
                eStatus = CEnums::eUntracked;
                bStaged = false;
            }
            else if (sStatus == sStatusOriginOfAdd)
            {
            }
        }

        CRepoFile* pFile = new CRepoFile();
        pFile->setFullName(sFullName);
        pFile->setFileName(sFileName);
        pFile->setRelativeName(sRelativeName);
        pFile->setStatus(eStatus);
        pFile->setStaged(bStaged);

        return pFile;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::onExecFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData)
{
    Q_UNUSED(sUserData);

    switch (eCommand)
    {

    case CEnums::eStageFile:
    case CEnums::eStageAll:
    case CEnums::eRevertFile:
    case CEnums::eCommit:
    case CEnums::eAmend:
    case CEnums::ePush:
    case CEnums::ePull:
    {
        // Throw the returned string of the process
        emit newOutputString(eCommand, sValue);
        break;
    }

    case CEnums::eAllFileStatus:
    case CEnums::eChangedFileStatus:
    {
        // Create CRepoFiles with the returned string of the process

        QList<CRepoFile*> lReturnValue;
        QStringList lStrings = sValue.split(NEW_LINE);

        for (QString sLine : lStrings)
        {
            CRepoFile* pFile = repoFileForLine(sPath, sLine);

            if (pFile != nullptr)
                lReturnValue << pFile;
        }

        emit newOutputListOfCRepoFile(eCommand, lReturnValue);
        break;
    }

    case CEnums::eFileLog:
    case CEnums::eBranchLog:
    {
        // Create CLogLines with the returned string of the process

        CLogLineCollection lReturnValue;
        QStringList lStrings = sValue.split(NEW_LINE);

        for (QString sLine : lStrings)
        {
            QStringList sValues = sLine.split(sLogFormatSplitter);

            if (sValues.count() == iLogFormatValueCount)
            {
                CLogLine* pLine = new CLogLine();

                pLine->setCommitId(sValues[0].trimmed());
                pLine->setMessage(sValues[1].trimmed());
                pLine->setAuthor(sValues[2].trimmed());
                pLine->setDate(QDateTime::fromString(sValues[3].trimmed(), Qt::ISODate));

                lReturnValue.add(pLine);
            }
        }

        emit newOutputCLogLineCollection(eCommand, lReturnValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CHgCommands::onNewOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
{
    emit newOutputListOfCRepoFile(eCommand, lNewRepoFiles);
}
