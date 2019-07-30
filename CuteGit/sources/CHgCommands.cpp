
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

    /  \.-"""-./  \
    \    -   -    /
     |   o   o   |
     \  .-'''-.  /
      '-\__Y__/-'
         `---`
*/

//-------------------------------------------------------------------------------------------------
// Command strings

static const char* sCommandGraph  = "hg log -G";
static const char* sCommandStatus = "hg status --ignored --porcelain";

//-------------------------------------------------------------------------------------------------
// Regular expressions

static const char* sStatusRegExp = "([a-zA-Z?!\\s])\\s(.*)";

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

CHgCommands::CHgCommands()
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
    exec(new CProcessCommand(CEnums::eAllFileStatus, sPath, sCommandStatus));
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CHgCommands::repoFileForLine(const QString &sPath, QString sLine)
{
    QRegExp tRegExp(sStatusRegExp);

    if (sLine.isEmpty() == false && sLine.back() == '/')
        sLine.chop(1);

    if (tRegExp.indexIn(sLine) != -1)
    {
        QString sStatus = tRegExp.cap(1).trimmed();
        QString sRelativeName = tRegExp.cap(2).split("->").last().trimmed();
        QString sFullName = sPath + "/" + sRelativeName;
        QString sFileName = QFileInfo(sFullName).fileName();
        bool bStaged = false;

        CEnums::ERepoFileStatus eStatus = CEnums::eClean;

        if (sStatus.isEmpty() == false)
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
                bStaged = false;
                eStatus = CEnums::eUntracked;
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

    case CEnums::eAllFileStatus:
    {
        // Create CRepoFiles with the returned string of the process

        QList<CRepoFile*> lReturnValue;
        QStringList lStrings = sValue.split("\n");

        for (QString sLine : lStrings)
        {
            CRepoFile* pFile = repoFileForLine(sPath, sLine);

            if (pFile != nullptr)
                lReturnValue << pFile;
        }

        emit newOutputListOfCRepoFile(eCommand, lReturnValue);
        break;
    }

    default:
    {
        break;
    }

    }
}
