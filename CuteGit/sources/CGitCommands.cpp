
// Qt
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCoreApplication>

// Application
#include "CUtils.h"
#include "CGitCommands.h"

/*!
    \class CGitCommands
    \inmodule CuteGit
    \section1 General
    This is the repository control interface for Git.

    \code

     _._     _,-'""`-._
    (,-.`._,'(       |\`-/|
        `-.-' \ )-`( , o o)
              `-    \`_`"'-

    \endcode
*/

//-------------------------------------------------------------------------------------------------
// Constants

const int CGitCommands::iLogFormatValueCount            = 4;
const int CGitCommands::iGraphFormatValueCount          = 5;

const QString CGitCommands::sCommandAbortMerge          = "git merge --abort";
const QString CGitCommands::sCommandAbortRebase         = "git rebase --abort";
const QString CGitCommands::sCommandAmend               = "git commit --amend --reset-author --no-edit";
const QString CGitCommands::sCommandBranchAhead         = "git rev-list --left-right --count \"%1\"...\"origin/%1\"";
const QString CGitCommands::sCommandBranches            = "git branch -a";
const QString CGitCommands::sCommandBranchFromCommit    = "git checkout -b \"%1\" \"%2\"";
const QString CGitCommands::sCommandBranchLog           = "git log --pretty=format:\"%h &&& %s &&& %an &&& %aI\" --max-count=20";
const QString CGitCommands::sCommandClone               = "git clone --progress \"%1\"";
const QString CGitCommands::sCommandCommit              = "git commit -m \"%1\"";
const QString CGitCommands::sCommandContinueMerge       = "git merge --continue";
const QString CGitCommands::sCommandContinueRebase      = "git rebase --continue";
const QString CGitCommands::sCommandCurrentBranch       = "git rev-parse --abbrev-ref HEAD";
const QString CGitCommands::sCommandDeleteBranch        = "git branch --delete \"%1\"";
const QString CGitCommands::sCommandFetch               = "git fetch";
const QString CGitCommands::sCommandFileLog             = "git log --pretty=format:\"%h &&& %s &&& %an &&& %aI\" --max-count=20 HEAD \"%1\"";
const QString CGitCommands::sCommandFileStatus          = "git status --porcelain --ignored --untracked-files=all \"%1\"";
const QString CGitCommands::sCommandGetRebaseApplyPath  = "git rev-parse --git-path rebase-apply";
const QString CGitCommands::sCommandGetRebaseMergePath  = "git rev-parse --git-path rebase-merge";
const QString CGitCommands::sCommandGraph               = "git log --graph --all --pretty=format:\"&&& %h &&& %s &&& %an &&& %aI\" --max-count=50";
const QString CGitCommands::sCommandHeadCommit          = "git rev-parse --short \"%1\"";
const QString CGitCommands::sCommandMergeBranch         = "git merge \"%1\"";
const QString CGitCommands::sCommandPull                = "git pull";
const QString CGitCommands::sCommandPush                = "git push";
const QString CGitCommands::sCommandRebaseOnCommit      = "git rebase --interactive %1~1";
const QString CGitCommands::sCommandResetOnCommit       = "git reset %1";
const QString CGitCommands::sCommandRevert              = "git checkout \"%1\"";
const QString CGitCommands::sCommandSetCurrentBranch    = "git checkout \"%1\"";
const QString CGitCommands::sCommandSquashCommit        = "git rebase --interactive %1~2";
const QString CGitCommands::sCommandStage               = "git add -f \"%1\"";
const QString CGitCommands::sCommandStageAll            = "git add -u";
const QString CGitCommands::sCommandStashPop            = "git stash pop";
const QString CGitCommands::sCommandStashSave           = "git stash save";
const QString CGitCommands::sCommandStatus              = "git status --porcelain --ignored --untracked-files=all";
const QString CGitCommands::sCommandTags                = "git tag";
const QString CGitCommands::sCommandTagCommit           = "git rev-parse --short \"%1\""; // "git rev-list --short -n 1 \"%1\"";
const QString CGitCommands::sCommandUnstage             = "git reset HEAD \"%1\"";
const QString CGitCommands::sCommandUnstageAll          = "git reset .";
const QString CGitCommands::sCommandUnstagedDiff        = "git diff --no-color --ignore-all-space HEAD \"%1\"";

const QString CGitCommands::sStatusBranchRegExp         = "";
const QString CGitCommands::sStatusRegExp               = "([a-zA-Z?!\\s])([a-zA-Z?!\\s])\\s(.*)";
const QString CGitCommands::sPickCommitRegExp           = "(pick)\\s+([a-zA-Z0-9]+)\\s+(.*)";

const QString CGitCommands::sLogFormatSplitter          = "&&&";
const QString CGitCommands::sRemoteBranchPrefix         = "remotes/origin/";
const QString CGitCommands::sSequenceEditorToken        = "GIT_SEQUENCE_EDITOR";
const QString CGitCommands::sTextEditorToken            = "GIT_EDITOR";
const QString CGitCommands::sRebaseEditCommit           = "edit %1 %2";
const QString CGitCommands::sRebaseRewordCommit         = "reword %1 %2";
const QString CGitCommands::sRebaseSquashCommit         = "squash %1 %2";
const QString CGitCommands::sComment                    = "#";

const QString CGitCommands::sStatusClean                = " ";
const QString CGitCommands::sStatusAdded                = "A";
const QString CGitCommands::sStatusModified             = "M";
const QString CGitCommands::sStatusRenamed              = "R";
const QString CGitCommands::sStatusDeleted              = "D";
const QString CGitCommands::sStatusUnmerged             = "U";
const QString CGitCommands::sStatusUntracked            = "?";
const QString CGitCommands::sStatusIgnored              = "!";

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

void CGitCommands::cloneRepository(const QString& sRepositoryURL, const QString& sRepositoryPath)
{
    QString sCommand = QString(sCommandClone).arg(sRepositoryURL);
    exec(new CProcessCommand(
             CEnums::eCloneRepository,
             sRepositoryPath,
             sCommand,
             false,
             QMap<QString, QString>(),
             "",
             CEnums::eCloneRepositoryFinished)
         );
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

void CGitCommands::allFileStatus(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eAllFileStatus, sPath, sCommandStatus));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::fileStatus(const QString& sPath, const QString& sFullName)
{
    QString sFileStatusCommand = QString(sCommandFileStatus).arg(sFullName);
    exec(new CProcessCommand(CEnums::eAllFileStatus, sPath, sFileStatusCommand));
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

void CGitCommands::branchCommitCountAheadBehind(const QString& sPath, QStringList lBranches)
{
    for (QString sBranch: lBranches)
    {
        if (not sBranch.startsWith(sRemoteBranchPrefix))
        {
            QString sCommand = QString(sCommandBranchAhead).arg(sBranch);
            exec(new CProcessCommand(CEnums::eBranchCommitCountAhead, sPath, sCommand, true, QMap<QString, QString>(), sBranch));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::tags(const QString& sPath)
{
    exec(new CProcessCommand(CEnums::eTags, sPath, sCommandTags));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::tagCommits(const QString& sPath, QStringList lTags)
{
    for (QString sTag: lTags)
    {
        QString sCommand = QString(sCommandTagCommit).arg(sTag);
        exec(new CProcessCommand(CEnums::eTagCommit, sPath, sCommand, true, QMap<QString, QString>(), sTag));
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
    QString sFileStatusCommand = QString(sCommandFileStatus).arg(sFullName);
    QString sLine = execNow(sPath, sFileStatusCommand);

    CRepoFile* pFile = repoFileForLine(sPath, sLine);

    if (pFile != nullptr)
    {
        stageFile(sPath, sFullName, not pFile->staged());
        delete pFile;
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
    emit newOutputString(CEnums::eNotification, bStage ? tr("Staging all files...") : tr("Unstaging all files..."));
    QString sCommand = QString(bStage ? sCommandStageAll : sCommandUnstageAll);
    exec(new CProcessCommand(CEnums::eStageAll, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    emit newOutputString(CEnums::eNotification, tr("Reverting..."));
    QString sCommand = QString(sCommandRevert).arg(sFullName);
    exec(new CProcessCommand(CEnums::eRevertFile, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commit(const QString& sPath, const QString& sMessage)
{
    emit newOutputString(CEnums::eNotification, tr("Commiting..."));
    QString sCommand = QString(sCommandCommit).arg(sMessage);
    exec(new CProcessCommand(CEnums::eCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::amend(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Amending..."));
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

void CGitCommands::stashSave(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Saving stash..."));
    exec(new CProcessCommand(CEnums::eStashSave, sPath, sCommandStashSave, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::stashPop(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Restoring stash..."));
    exec(new CProcessCommand(CEnums::eStashPop, sPath, sCommandStashPop, true));
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
    emit newOutputString(CEnums::eNotification, QString(tr("Switching to %1...")).arg(sBranch));
    QString sFinalName = sBranch;
    sFinalName.replace(sRemoteBranchPrefix, "");
    QString sCommand = QString(sCommandSetCurrentBranch).arg(sFinalName);
    exec(new CProcessCommand(CEnums::eSetCurrentBranch, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitReset(const QString& sPath, const QString& sCommitId)
{
    emit newOutputString(CEnums::eNotification, QString(tr("Doing reset on %1...")).arg(sCommitId));

    QString sCommand = QString(sCommandResetOnCommit).arg(sCommitId);
    exec(new CProcessCommand(CEnums::eResetToCommit, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitRebase(const QString& sPath, const QString& sCommitId)
{
    emit newOutputString(CEnums::eNotification, QString(tr("Doing rebase on %1...")).arg(sCommitId));

    m_eRebaseType = eRTEdit;
    m_eRebaseStep = eRSChangeCommitEditSequence;
    m_sCommitId = sCommitId;
    m_sCommitMessage.clear();

    QMap<QString, QString> mEnvironment;
    mEnvironment[sSequenceEditorToken] = QCoreApplication::applicationFilePath();

    QString sCommand = QString(sCommandRebaseOnCommit).arg(sCommitId);

    exec(new CProcessCommand(CEnums::eRebaseOnCommit, sPath, sCommand, true, mEnvironment));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::commitSquash(const QString& sPath, const QString& sCommitId)
{
    emit newOutputString(CEnums::eNotification, QString(tr("Squashing %1...")).arg(sCommitId));

    m_eRebaseType = eRTSquash;
    m_eRebaseStep = eRSSquashCommitEditSequence;
    m_sCommitId = sCommitId;
    m_sCommitMessage.clear();

    QMap<QString, QString> mEnvironment;
    mEnvironment[sSequenceEditorToken] = QCoreApplication::applicationFilePath();
    mEnvironment[sTextEditorToken] = QCoreApplication::applicationFilePath();

    QString sCommand = QString(sCommandSquashCommit).arg(sCommitId);

    exec(new CProcessCommand(CEnums::eSquashCommit, sPath, sCommand, true, mEnvironment));
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

void CGitCommands::mergeBranch(const QString& sPath, const QString& sBranchName)
{
    emit newOutputString(CEnums::eNotification, QString(tr("Merging branch %1...")).arg(sBranchName));
    QString sCommand = QString(sCommandMergeBranch).arg(sBranchName);
    exec(new CProcessCommand(CEnums::eMergeBranch, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::deleteBranch(const QString& sPath, const QString& sBranchName)
{
    emit newOutputString(CEnums::eNotification, QString(tr("Deleting branch %1...")).arg(sBranchName));
    QString sCommand = QString(sCommandDeleteBranch).arg(sBranchName);
    exec(new CProcessCommand(CEnums::eDeleteBranch, sPath, sCommand, true));
}

//-------------------------------------------------------------------------------------------------

void CGitCommands::editSequenceFile(const QString& sFileName)
{
    // Read in the file provided by GIT
    QString sInputText = CUtils::getTextFileContents(sFileName);

    if (not sInputText.isEmpty())
    {
        QStringList lOutputLines;

        // Get a list of lines
        QStringList lLines = sInputText.split(NEW_LINE);

        switch (m_eRebaseStep)
        {
        // In case we're modifying the sequence file
        // Most often .git/rebase-merge/git-rebase-todo
        case eRSChangeCommitEditSequence:
        case eRSSquashCommitEditSequence:
        {
            // Setup a "pick nnnn aaaa" reg exp
            QRegExp tRegExp(sPickCommitRegExp);

            for (QString sLine : lLines)
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
                                m_eRebaseStep = eRSChangeCommitEditMessage;
                                break;
                            }

                            case eRTSquash:
                            {
                                lOutputLines << QString(sRebaseSquashCommit).arg(sCommitID).arg(sMessage);
                                m_eRebaseStep = eRSSquashCommitEditMessage;
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

            break;
        }

        case eRSChangeCommitEditMessage:
        {
            lOutputLines = m_sCommitMessage.split(NEW_LINE);
            break;
        }

        case eRSSquashCommitEditMessage:
        {
            lOutputLines = lLines;
            break;
        }
        }

        // Rebuild output text from lines
        QString sOutputText;

        for (QString sLine : lOutputLines)
        {
            sOutputText += sLine;
            sOutputText += NEW_LINE;
        }

        QFile file(sFileName);

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

#ifdef HAVE_QSTRING_BACK
    if (not sLine.isEmpty() && sLine.back() == '/')
        sLine.chop(1);
#endif

    // TODO : Handle quoted file names

    if (tRegExp.indexIn(sLine) != -1)
    {
        QString sStaged = tRegExp.cap(1).trimmed();
        QString sUnstaged = tRegExp.cap(2).trimmed();
        QString sRelativeName = tRegExp.cap(3).split("->").last().trimmed();
        QString sFullName = sPath + PATH_SEP + sRelativeName;
        QString sFileName = QFileInfo(sFullName).fileName();
        bool bStaged = false;

        CEnums::ERepoFileStatus eStatus = CEnums::eClean;

        if (not sStaged.isEmpty())
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

        if (not sUnstaged.isEmpty())
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
            {
                eStatus = CEnums::eIgnored;
            }
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

    case CEnums::eNothing:
    case CEnums::eNotification:
    case CEnums::eCurrentBranch:
    case CEnums::eRepositoryStatus:
        break;

    case CEnums::eCloneRepository:
    case CEnums::eCloneRepositoryFinished:
    case CEnums::eStageFile:
    case CEnums::eStageAll:
    case CEnums::eRevertFile:
    case CEnums::eCommit:
    case CEnums::eAmend:
    case CEnums::ePush:
    case CEnums::ePull:
    case CEnums::eFetch:
    case CEnums::eStashSave:
    case CEnums::eStashPop:
    case CEnums::eSetCurrentBranch:
    case CEnums::eResetToCommit:
    case CEnums::eRebaseOnCommit:
    case CEnums::eSquashCommit:
    case CEnums::eBranchFromCommit:
    case CEnums::eMergeBranch:
    case CEnums::eDeleteBranch:
    case CEnums::eChangeCommitMessage:
    case CEnums::eContinueRebase:
    case CEnums::eAbortRebase:
    {
        // Throw the returned string of the process
        emit newOutputString(eCommand, sValue);
        break;
    }

    case CEnums::eBranchHeadCommit:
    case CEnums::eTagCommit:
    {
        // Throw the returned string of the process
        emit newOutputKeyValue(eCommand, sUserData, sValue.trimmed());
        break;
    }

    // toto
    case CEnums::eBranchCommitCountAhead:
    case CEnums::eBranchCommitCountBehind:
    {
        QStringList sAheadOutputLines = sValue.split("\t");
        QString sAheadCount = sAheadOutputLines.count() > 0 ? sAheadOutputLines[0].trimmed() : "0";
        QString sBehindCount = sAheadOutputLines.count() > 1 ? sAheadOutputLines[1].trimmed() : "0";

        emit newOutputKeyValue(CEnums::eBranchCommitCountAhead, sUserData, sAheadCount);
        emit newOutputKeyValue(CEnums::eBranchCommitCountBehind, sUserData, sBehindCount);

        break;
    }

    case CEnums::eUnstagedFileDiff:
    {
        // Create CDiffLines with the returned string of the process

        QList<CDiffLine*> lReturnValue;
        QStringList lLines = sValue.split(NEW_LINE);
        bool bAtLeastOneLineOK = false;

        for (QString sLine : lLines)
        {
            QString sTrimmedLine = sLine.trimmed();

            if (not sTrimmedLine.isEmpty())
            {
                bAtLeastOneLineOK = true;

                CDiffLine* pDiffLine = new CDiffLine();
                pDiffLine->setText(sLine);

                if (sLine.startsWith("+"))
                    pDiffLine->setOperation(CEnums::Add);
                if (sLine.startsWith("-"))
                    pDiffLine->setOperation(CEnums::Delete);

                lReturnValue << pDiffLine;
            }
        }

        if (not bAtLeastOneLineOK)
        {
            qDeleteAll(lReturnValue);
            lReturnValue.clear();
        }

        emit newOutputListOfCDiffLine(eCommand, lReturnValue);
        break;
    }

    case CEnums::eBranches:
    {
        // Create CBranchs with the returned string of the process

        QList<CBranch*> lReturnValue;
        QStringList lLines = sValue.split(NEW_LINE);

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

                    // Tell the world about the current branch
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

    case CEnums::eTags:
    {
        QList<CBranch*> lReturnValue;
        QStringList lLines = sValue.split(NEW_LINE);

        for (QString sLine : lLines)
        {
            sLine = sLine.trimmed();

            if (not sLine.isEmpty())
            {
                CBranch* pNewBranch = new CBranch();
                pNewBranch->setName(sLine);
                lReturnValue << pNewBranch;
            }
        }

        emit newOutputListOfCBranch(eCommand, lReturnValue);
        break;
    }

    case CEnums::eAllFileStatus:
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

        QList<CLogLine*> lReturnValue;
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

                lReturnValue << pLine;
            }
        }

        emit newOutputListOfCLogLine(eCommand, lReturnValue);
        break;
    }

    case CEnums::eGraph:
    {
        // Create CGraphLines with the returned string of the process

        QList<CGraphLine*> lReturnValue;
        QStringList lStrings = sValue.split(NEW_LINE);

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
            else if (sValues.count() == 1)
            {
                CGraphLine* pLine = new CGraphLine();

                pLine->setGraphSymbol(sValues[0].trimmed());

                lReturnValue << pLine;
            }
        }

        emit newOutputListOfCGraphLine(eCommand, lReturnValue);
        break;
    }

    }
}
