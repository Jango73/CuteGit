
// Qt
#include <QDebug>

// Application
#include "CSvnCommands.h"

//-------------------------------------------------------------------------------------------------

CSvnCommands::CSvnCommands()
{
    connect(this, &CCommands::execFinished, this, &CSvnCommands::onExecFinished);
}

//-------------------------------------------------------------------------------------------------

CSvnCommands::~CSvnCommands()
{
}

//-------------------------------------------------------------------------------------------------

bool CSvnCommands::can(ECapability eWhat) const
{
    switch (eWhat)
    {
        case Clone:
        case ShowGlobalGraph:
        case ShowBranchLog:
        case ShowFileLog:
        case Commit:
        case Pull:
            return true;

    default:
        return false;
    }
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::allFileStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::repositoryStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::branches(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::graph(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    Q_UNUSED(sPath);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::fileLog(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::toggleStaged(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::stageAll(const QString& sPath, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::commit(const QString& sPath, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::pull(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::setCurrentBranch(const QString& sPath, const QString& sBranch)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sBranch);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::onExecFinished(QString sPath, CProcessCommand::EProcessCommand eCommand, QString sValue)
{
    Q_UNUSED(sPath);

    switch (eCommand)
    {

    case CProcessCommand::eNotification:
    case CProcessCommand::eCurrentBranch:
    case CProcessCommand::eRepositoryStatus:
        break;

    case CProcessCommand::eStageFile:
    case CProcessCommand::eStageAll:
    case CProcessCommand::eRevertFile:
    case CProcessCommand::eCommit:
    case CProcessCommand::eAmend:
    case CProcessCommand::ePush:
    case CProcessCommand::ePull:
    case CProcessCommand::eSetCurrentBranch:
    case CProcessCommand::eCommitReset:
    case CProcessCommand::eCommitRebase:
    case CProcessCommand::eCommitSquash:
    case CProcessCommand::eChangeCommitMessage:
    case CProcessCommand::eContinueRebase:
    case CProcessCommand::eAbortRebase:
    {
        emit newOutputString(eCommand, sValue);
        break;
    }

    case CProcessCommand::eUnstagedFileDiff:
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
                    pDiffLine->setOperation(CDiffLine::Add);
                if (sLine.startsWith("-"))
                    pDiffLine->setOperation(CDiffLine::Delete);

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

    case CProcessCommand::eBranches:
    {
        QStringList lReturnValue;
        emit newOutputStringList(eCommand, lReturnValue);
        break;
    }

    case CProcessCommand::eAllFileStatus:
    {
        QList<CRepoFile*> lReturnValue;
        emit newOutputListOfCRepoFile(eCommand, lReturnValue);
        break;
    }

    case CProcessCommand::eFileLog:
    case CProcessCommand::eBranchLog:
    {
        QList<CLogLine*> lReturnValue;
        emit newOutputListOfCLogLine(eCommand, lReturnValue);
        break;
    }

    case CProcessCommand::eGraph:
    {
        QList<CGraphLine*> lReturnValue;
        emit newOutputListOfCGraphLine(eCommand, lReturnValue);
        break;
    }

    }
}
