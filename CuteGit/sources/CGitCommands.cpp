
// Qt
#include <QDebug>
#include <QRegExp>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

static const char* sCommandStatus = "git status --ignored --porcelain";
static const char* sCommandBranches = "git branch -a";
// static const char* sCommandGraph = "git log --graph --pretty=format:\"%h | %s | %an | %ai\" --after=\"%1\" --before=\"%2\"";
static const char* sCommandGraph = "git log --graph --pretty=format:\"%h | %s | %an | %ai\" --max-count=20";
static const char* sCommandFileLog = "git log --max-count=20 \"%1\"";
static const char* sCommandStage = "git add -f \"%1\"";
static const char* sCommandUnstage = "git reset \"%1\"";
static const char* sCommandStageAll = "git add -u";
static const char* sCommandUnstageAll = "git reset .";
static const char* sCommandRevert = "git checkout \"%1\"";
static const char* sCommandCommit = "git commit -m \"%1\"";
static const char* sCommandPush = "git push";
static const char* sCommandPull = "git pull";
static const char* sCommandUnstagedDiff = "git diff --no-color --ignore-all-space \"%1\"";

static const char* sStatusRegExp = "([a-zA-Z?!\\s])([a-zA-Z?!\\s])\\s(.*)";

const QString sStatusAdded = "A";
const QString sStatusModified = "M";
const QString sStatusDeleted = "D";
const QString sStatusUntracked = "?";
const QString sStatusIgnored = "!";

//-------------------------------------------------------------------------------------------------

CGitCommands::CGitCommands()
{
    connect(this, &CCommands::execFinished, this, &CGitCommands::onExecFinished);
}

//-------------------------------------------------------------------------------------------------

CGitCommands::~CGitCommands()
{
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::allFileStatus(const QString& sPath)
{
    exec(new CProcessCommand(CProcessCommand::eAllFileStatus, sPath, sCommandStatus));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::branches(const QString& sPath)
{
    exec(new CProcessCommand(CProcessCommand::eBranches, sPath, sCommandBranches));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::graph(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    QString sFrom = from.toString(Qt::ISODate);
    QString sTo = to.toString(Qt::ISODate);
    QString sCommand = QString(sCommandGraph); // .arg(sFrom).arg(sTo);
    exec(new CProcessCommand(CProcessCommand::eGraph, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::fileLog(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandFileLog).arg(sFullName);
    exec(new CProcessCommand(CProcessCommand::eFileLog, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandStage : sCommandUnstage).arg(sFullName);
    exec(new CProcessCommand(CProcessCommand::eStageFile, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::stageAll(const QString& sPath, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandStageAll : sCommandUnstageAll);
    exec(new CProcessCommand(CProcessCommand::eStageAll, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandRevert).arg(sFullName);
    exec(new CProcessCommand(CProcessCommand::eRevertFile, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commit(const QString& sPath, const QString& sMessage)
{
    QString sCommand = QString(sCommandCommit).arg(sMessage);
    exec(new CProcessCommand(CProcessCommand::eCommit, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::push(const QString& sPath)
{
    QString sCommand = QString(sCommandPush);
    exec(new CProcessCommand(CProcessCommand::ePush, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::pull(const QString& sPath)
{
    QString sCommand = QString(sCommandPull);
    exec(new CProcessCommand(CProcessCommand::ePull, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandUnstagedDiff).arg(sFullName);
    exec(new CProcessCommand(CProcessCommand::eUnstagedFileDiff, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::onExecFinished(QString sPath, CProcessCommand::EProcessCommand eCommand, QString sValue)
{
    switch (eCommand)
    {

    case CProcessCommand::eAllFileStatus:
    {
        QList<CRepoFile*> vReturnValue;
        QStringList lStrings = sValue.split("\n");
        QRegExp tRegExp(sStatusRegExp);

        for (QString sLine : lStrings)
        {
            if (sLine.isEmpty() == false && sLine.back() == '/')
                sLine.chop(1);

            if (tRegExp.indexIn(sLine) != -1)
            {
                QString sStaged = tRegExp.cap(1).trimmed();
                QString sUnstaged = tRegExp.cap(2).trimmed();
                QString sFullName = sPath + "/" + tRegExp.cap(3).trimmed();
                bool bStaged = false;

                CRepoFile::ERepoFileStatus eStatus = CRepoFile::eClean;

                if (sStaged.isEmpty() == false)
                {
                    bStaged = true;

                    if (sStaged == sStatusAdded)
                        eStatus = CRepoFile::eAdded;
                    else if (sStaged == sStatusModified)
                        eStatus = CRepoFile::eModified;
                    else if (sStaged == sStatusDeleted)
                        eStatus = CRepoFile::eDeleted;
                    else if (sStaged == sStatusIgnored)
                        eStatus = CRepoFile::eIgnored;
                    else if (sStaged == sStatusUntracked)
                    {
                        bStaged = false;
                        eStatus = CRepoFile::eUntracked;
                    }
                }

                if (sUnstaged.isEmpty() == false)
                {
                    if (sUnstaged == sStatusAdded)
                    {
                        bStaged = false;
                        eStatus = CRepoFile::eAdded;
                    }
                    else if (sUnstaged == sStatusModified)
                    {
                        bStaged = false;
                        eStatus = CRepoFile::eModified;
                    }
                    else if (sUnstaged == sStatusDeleted)
                    {
                        bStaged = false;
                        eStatus = CRepoFile::eDeleted;
                    }
                    else if (sUnstaged == sStatusIgnored)
                        eStatus = CRepoFile::eIgnored;
                    else if (sUnstaged == sStatusUntracked)
                    {
                        bStaged = false;
                        eStatus = CRepoFile::eUntracked;
                    }
                }

                CRepoFile* pFile = new CRepoFile();
                pFile->setStatus(eStatus);
                pFile->setFullName(sFullName);
                pFile->setStaged(bStaged);

                vReturnValue << pFile;
            }
        }

        emit execFinished_QList_CRepoFile(eCommand, vReturnValue);
        break;
    }

    case CProcessCommand::eBranches:
    case CProcessCommand::eGraph:
    case CProcessCommand::eFileLog:
    case CProcessCommand::eUnstagedFileDiff:
    {
        QStringList lReturnValue = sValue.split("\n");

        emit execFinished_QStringList(eCommand, lReturnValue);
        break;
    }

    case CProcessCommand::eStageFile:
    case CProcessCommand::eStageAll:
    case CProcessCommand::eRevertFile:
    case CProcessCommand::eCommit:
    case CProcessCommand::ePush:
    case CProcessCommand::ePull:
    {
        emit execFinished_QString(eCommand, sValue);
        break;
    }

    }
}
