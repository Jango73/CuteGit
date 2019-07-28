
// Qt
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCoreApplication>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

static int iLogFormatValueCount = 4;
static int iGraphFormatValueCount = 5;
static const char* sLogFormatSplitter = "&&&";

static const char* sCommandStatus = "git status --ignored --porcelain";
static const char* sCommandStatusForFile = "git status --ignored --porcelain \"%1\"";
static const char* sCommandBranches = "git branch -a";

static const char* sCommandBranchLog = "git log --pretty=format:\"%H &&& %s &&& %an &&& %aI\" --max-count=20";
static const char* sCommandFileLog = "git log --pretty=format:\"%H &&& %s &&& %an &&& %aI\" --max-count=20 HEAD \"%1\"";
static const char* sCommandUnstagedDiff = "git diff --no-color --ignore-all-space HEAD \"%1\"";
static const char* sCommandGraph = "git log --graph --all --pretty=format:\"&&& %h &&& %s &&& %an &&& %aI\"";
static const char* sCommandHeadCommit = "git rev-parse \"%1\"";

static const char* sCommandStage = "git add -f \"%1\"";
static const char* sCommandUnstage = "git reset HEAD \"%1\"";
static const char* sCommandStageAll = "git add -u";
static const char* sCommandUnstageAll = "git reset .";
static const char* sCommandRevert = "git checkout \"%1\"";
static const char* sCommandCommit = "git commit -m \"%1\"";
static const char* sCommandAmend = "git commit --amend --reset-author --no-edit";
static const char* sCommandPush = "git push";
static const char* sCommandPull = "git pull";
static const char* sCommandFetch = "git fetch";
static const char* sCommandSetCurrentBranch = "git checkout \"%1\"";
static const char* sCommandResetOnCommit = "git reset %1";
static const char* sCommandRebaseOnCommit = "git rebase --interactive %1~1";
static const char* sCommandContinueRebase = "git rebase --continue";
static const char* sCommandAbortRebase = "git rebase --abort";
static const char* sCommandBranchFromCommit = "git checkout -b \"%1\" \"%2\"";

static const char* sCommandGetRebaseApplyPath = "git rev-parse --git-path rebase-apply";
static const char* sCommandGetRebaseMergePath = "git rev-parse --git-path rebase-merge";

static const char* sStatusRegExp = "([a-zA-Z?!\\s])([a-zA-Z?!\\s])\\s(.*)";
static const char* sPickCommitRegExp = "(pick)\\s+([a-zA-Z0-9]+)\\s+(.*)";

static const char* sRemoteBranchPrefix = "remotes/origin/";

static const char* sSequenceEditorToken = "GIT_SEQUENCE_EDITOR";
static const char* sTextEditorToken = "GIT_EDITOR";

static const char* sRebaseEditCommit = "edit %1 %2";
static const char* sRebaseRewordCommit = "reword %1 %2";

static const char* sComment = "#";

const QString sStatusAdded = "A";
const QString sStatusModified = "M";
const QString sStatusRenamed = "R";
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

bool CGitCommands::can(CEnums::ECapability eWhat) const
{
    Q_UNUSED(eWhat);

    // Yes Git does all this + coffee
    return true;
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::allFileStatus(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eAllFileStatus, sPath, sCommandStatus));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::repositoryStatus(const QString& sPath)
{
    QString sRebaseApplyPath = QString("%1/%2").arg(sPath).arg(execNow(sPath, sCommandGetRebaseApplyPath).trimmed());
    QString sRebaseMergePath = QString("%1/%2").arg(sPath).arg(execNow(sPath, sCommandGetRebaseMergePath).trimmed());

    if (QDir(sRebaseMergePath).exists())
    {
        emit newOutputString(
                    CEnums::eRepositoryStatus,
                    CRepoFile::sRepositoryStatusInteractiveRebase
                    );
    }
    else if (QDir(sRebaseApplyPath).exists())
    {
        emit newOutputString(
                    CEnums::eRepositoryStatus,
                    CRepoFile::sRepositoryStatusRebase
                    );
    }
    else
    {
        emit newOutputString(
                    CEnums::eRepositoryStatus,
                    CRepoFile::sRepositoryStatusClean
                    );
    }
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::branches(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eBranches, sPath, sCommandBranches));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::branchHeadCommits(const QString& sPath, QStringList lBranches)
{
    for (QString sBranch: lBranches)
    {
        QString sCommand = QString(sCommandHeadCommit).arg(sBranch);
        exec(new CProcessCommand(CEnums::eBranchHeadCommit, sPath, sCommand, true, QMap<QString, QString>(), sBranch));
    }
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::graph(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eGraph, sPath, sCommandGraph));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    QString sFrom = from.toString(Qt::ISODate);
    QString sTo = to.toString(Qt::ISODate);
    QString sCommand = QString(sCommandBranchLog); // .arg(sFrom).arg(sTo);
    exec(new CProcessCommand(CEnums::eBranchLog, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::fileLog(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandFileLog).arg(sFullName);
    exec(new CProcessCommand(CEnums::eFileLog, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::toggleStaged(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandStatusForFile).arg(sFullName);
    QString sLine = execNow(sPath, sCommand);

    CRepoFile* pFile = repoFileForLine(sPath, sLine);

    if (pFile != nullptr)
    {
        stageFile(sPath, sFullName, not pFile->staged());
        pFile->deleteLater();
    }
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandStage : sCommandUnstage).arg(sFullName);
    exec(new CProcessCommand(CEnums::eStageFile, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::stageAll(const QString& sPath, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandStageAll : sCommandUnstageAll);
    exec(new CProcessCommand(CEnums::eStageAll, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandRevert).arg(sFullName);
    exec(new CProcessCommand(CEnums::eRevertFile, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commit(const QString& sPath, const QString& sMessage)
{
    QString sCommand = QString(sCommandCommit).arg(sMessage);
    exec(new CProcessCommand(CEnums::eCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::amend(const QString& sPath)
{
    QString sCommand = QString(sCommandAmend);
    exec(new CProcessCommand(CEnums::eAmend, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::push(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Pushing..."));
    QString sCommand = QString(sCommandPush);
    exec(new CProcessCommand(CEnums::ePush, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::pull(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Pulling..."));
    QString sCommand = QString(sCommandPull);
    exec(new CProcessCommand(CEnums::ePull, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::fetch(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Fetching..."));
    QString sCommand = QString(sCommandFetch);
    exec(new CProcessCommand(CEnums::eFetch, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    QString sCommand = QString(sCommandUnstagedDiff).arg(sFullName);
    exec(new CProcessCommand(CEnums::eUnstagedFileDiff, sPath, sCommand));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::setCurrentBranch(const QString& sPath, const QString& sBranch)
{
    QString sFinalName = sBranch;
    sFinalName.replace(sRemoteBranchPrefix, "");
    QString sCommand = QString(sCommandSetCurrentBranch).arg(sFinalName);
    exec(new CProcessCommand(CEnums::eSetCurrentBranch, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitReset(const QString& sPath, const QString& sCommitId)
{
    emit newOutputString(CEnums::eNotification, tr("Doing reset..."));

    QString sCommand = QString(sCommandResetOnCommit).arg(sCommitId);
    exec(new CProcessCommand(CEnums::eResetToCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitRebase(const QString& sPath, const QString& sCommitId)
{
    emit newOutputString(CEnums::eNotification, tr("Doing rebase..."));

    m_eRebaseType = eRTEdit;
    m_eRebaseStep = eRSChangeCommitEditSequence;
    m_sCommitId = sCommitId;
    m_sCommitMessage = "";

    QMap<QString, QString> mEnvironment;
    mEnvironment[sSequenceEditorToken] = QCoreApplication::applicationFilePath();

    QString sCommand = QString(sCommandRebaseOnCommit).arg(sCommitId);

    exec(new CProcessCommand(CEnums::eRebaseOnCommit, sPath, sCommand, true, mEnvironment));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitSquash(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);

    // TODO
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitBranchFrom(const QString& sPath, const QString& sCommitId, const QString& sBranchName)
{
    QString sCommand = QString(sCommandBranchFromCommit).arg(sBranchName).arg(sCommitId);
    exec(new CProcessCommand(CEnums::eBranchFromCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage)
{
    emit newOutputString(CEnums::eNotification, tr("Changing commit message..."));

    m_eRebaseType = eRTReword;
    m_eRebaseStep = eRSChangeCommitEditSequence;
    m_sCommitId = sCommitId;
    m_sCommitMessage = sMessage;

    QMap<QString, QString> mEnvironment;
    mEnvironment[sSequenceEditorToken] = QCoreApplication::applicationFilePath();
    mEnvironment[sTextEditorToken] = QCoreApplication::applicationFilePath();

    QString sCommand = QString(sCommandRebaseOnCommit).arg(sCommitId);

    exec(new CProcessCommand(CEnums::eChangeCommitMessage, sPath, sCommand, true, mEnvironment));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::continueRebase(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Continuing rebase..."));
    QString sCommand = QString(sCommandContinueRebase);
    exec(new CProcessCommand(CEnums::eContinueRebase, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::abortRebase(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Aborting rebase..."));
    QString sCommand = QString(sCommandAbortRebase);
    exec(new CProcessCommand(CEnums::eAbortRebase, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::editSequenceFile(const QString& sFileName)
{
    QFile file(sFileName);

    if (file.open(QIODevice::ReadOnly))
    {
        // Read in the file provided by GIT
        QTextStream stream(&file);
        QString sInputText = stream.readAll();
        QStringList lOutputLines;
        file.close();

        // Get a list of lines
        QStringList sLines = sInputText.split("\n");

        switch (m_eRebaseStep)
        {
        // In case we're modifyng the sequence file
        // Most often .git/rebase-merge/git-rebase-todo
        case eRSChangeCommitEditSequence:
        {
            // Setup a "pick nnnn aaaa" reg exp
            QRegExp tRegExp(sPickCommitRegExp);

            for (QString sLine : sLines)
            {
                // Process only non-comment lines
                if (not sLine.startsWith(sComment))
                {
                    if (tRegExp.indexIn(sLine) != -1)
                    {
                        QString sWord = tRegExp.cap(1).trimmed();
                        QString sCommitID = tRegExp.cap(2).trimmed();
                        QString sMessage = tRegExp.cap(3).trimmed();

                        if (sCommitID == m_sCommitId)
                        {
                            switch (m_eRebaseType)
                            {

                            case eRTEdit:
                            {
                                lOutputLines << QString(sRebaseEditCommit).arg(sCommitID).arg(sMessage);
                                break;
                            }

                            case eRTReword:
                            {
                                lOutputLines << QString(sRebaseRewordCommit).arg(sCommitID).arg(sMessage);
                                break;
                            }

                            }
                        }
                        else
                        {
                            lOutputLines << sLine;
                        }
                    }
                }
            }

            m_eRebaseStep = eRSChangeCommitEditMessage;
            break;
        }

        case eRSChangeCommitEditMessage:
        {
            lOutputLines = m_sCommitMessage.split("\n");
            break;
        }
        }

        // Rebuild output text from lines
        QString sOutputText;

        for (QString sLine : lOutputLines)
        {
            sOutputText += sLine;
            sOutputText += "\n";
        }

        // Rewrite sequence file
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << sOutputText;
            file.close();
        }
    }
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CGitCommands::repoFileForLine(const QString &sPath, QString sLine)
{
    QRegExp tRegExp(sStatusRegExp);

    if (sLine.isEmpty() == false && sLine.back() == '/')
        sLine.chop(1);

    if (tRegExp.indexIn(sLine) != -1)
    {
        QString sStaged = tRegExp.cap(1).trimmed();
        QString sUnstaged = tRegExp.cap(2).trimmed();
        QString sRelativeName = tRegExp.cap(3).split("->").last().trimmed();
        QString sFullName = sPath + "/" + sRelativeName;
        QString sFileName = QFileInfo(sFullName).fileName();
        bool bStaged = false;

        CEnums::ERepoFileStatus eStatus = CEnums::eClean;

        if (sStaged.isEmpty() == false)
        {
            bStaged = true;

            if (sStaged == sStatusAdded)
                eStatus = CEnums::eAdded;
            else if (sStaged == sStatusModified)
                eStatus = CEnums::eModified;
            else if (sStaged == sStatusRenamed)
                eStatus = CEnums::eRenamed;
            else if (sStaged == sStatusDeleted)
                eStatus = CEnums::eDeleted;
            else if (sStaged == sStatusIgnored)
                eStatus = CEnums::eIgnored;
            else if (sStaged == sStatusUntracked)
            {
                bStaged = false;
                eStatus = CEnums::eUntracked;
            }
        }

        if (sUnstaged.isEmpty() == false)
        {
            if (sUnstaged == sStatusAdded)
            {
                bStaged = false;
                eStatus = CEnums::eAdded;
            }
            else if (sUnstaged == sStatusModified)
            {
                bStaged = false;
                eStatus = CEnums::eModified;
            }
            else if (sUnstaged == sStatusRenamed)
            {
                bStaged = false;
                eStatus = CEnums::eRenamed;
            }
            else if (sUnstaged == sStatusDeleted)
            {
                bStaged = false;
                eStatus = CEnums::eDeleted;
            }
            else if (sUnstaged == sStatusIgnored)
                eStatus = CEnums::eIgnored;
            else if (sUnstaged == sStatusUntracked)
            {
                bStaged = false;
                eStatus = CEnums::eUntracked;
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

void CGitCommands::onExecFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData)
{
    switch (eCommand)
    {

    case CEnums::eNotification:
    case CEnums::eCurrentBranch:
    case CEnums::eRepositoryStatus:
        break;

    case CEnums::eStageFile:
    case CEnums::eStageAll:
    case CEnums::eRevertFile:
    case CEnums::eCommit:
    case CEnums::eAmend:
    case CEnums::ePush:
    case CEnums::ePull:
    case CEnums::eFetch:
    case CEnums::eSetCurrentBranch:
    case CEnums::eResetToCommit:
    case CEnums::eRebaseOnCommit:
    case CEnums::eSquashCommit:
    case CEnums::eBranchFromCommit:
    case CEnums::eChangeCommitMessage:
    case CEnums::eContinueRebase:
    case CEnums::eAbortRebase:
    {
        emit newOutputString(eCommand, sValue);
        break;
    }

    case CEnums::eBranchHeadCommit:
    {
        emit newOutputKeyValue(eCommand, sUserData, sValue.trimmed());
        break;
    }

    case CEnums::eUnstagedFileDiff:
    {
        QList<CDiffLine*> lReturnValue;
        QStringList lLines = sValue.split("\n");
        bool bAtLeastOneLineNotEmpty = false;

        for (QString sLine : lLines)
        {
            QString sTrimmedLine = sLine.trimmed();

            if (not sTrimmedLine.isEmpty())
            {
                bAtLeastOneLineNotEmpty = true;

                CDiffLine* pDiffLine = new CDiffLine();
                pDiffLine->setText(sLine);

                if (sLine.startsWith("+"))
                    pDiffLine->setOperation(CEnums::Add);
                if (sLine.startsWith("-"))
                    pDiffLine->setOperation(CEnums::Delete);

                lReturnValue << pDiffLine;
            }
        }

        if (bAtLeastOneLineNotEmpty == false)
        {
            qDeleteAll(lReturnValue);
            lReturnValue.clear();
        }

        emit newOutputListOfCDiffLine(eCommand, lReturnValue);
        break;
    }

    case CEnums::eBranches:
    {
        QStringList lLines = sValue.split("\n");
        QList<CBranch*> lReturnValue;

        for (QString sLine : lLines)
        {
            sLine = sLine.split("->").first();

            if (not sLine.isEmpty())
            {
                CBranch* pNewBranch = new CBranch();

                if (sLine.startsWith("*"))
                {
                    sLine.remove(0, 2);
                    pNewBranch->setName(sLine);

                    emit newOutputString(CEnums::eCurrentBranch, sLine);
                }
                else
                {
                    sLine.remove(0, 2);
                    pNewBranch->setName(sLine);
                }

                lReturnValue << pNewBranch;
            }
        }

        emit newOutputListOfCBranch(eCommand, lReturnValue);
        break;
    }

    case CEnums::eAllFileStatus:
    {
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

    case CEnums::eFileLog:
    case CEnums::eBranchLog:
    {
        QList<CLogLine*> lReturnValue;
        QStringList lStrings = sValue.split("\n");

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

                lReturnValue << pLine;
            }
        }

        emit newOutputListOfCLogLine(eCommand, lReturnValue);
        break;
    }

    case CEnums::eGraph:
    {
        QList<CGraphLine*> lReturnValue;
        QStringList lStrings = sValue.split("\n");

        for (QString sLine : lStrings)
        {
            QStringList sValues = sLine.split(sLogFormatSplitter);

            if (sValues.count() == iGraphFormatValueCount)
            {
                CGraphLine* pLine = new CGraphLine();

                pLine->setGraphSymbol(sValues[0].trimmed());
                pLine->setCommitId(sValues[1].trimmed());
                pLine->setMessage(sValues[2].trimmed());
                pLine->setAuthor(sValues[3].trimmed());
                pLine->setDate(QDateTime::fromString(sValues[4].trimmed(), Qt::ISODate));

                lReturnValue << pLine;
            }
        }

        emit newOutputListOfCGraphLine(eCommand, lReturnValue);
        break;
    }

    }
}
