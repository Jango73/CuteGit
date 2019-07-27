
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

bool CSvnCommands::can(CEnums::ECapability eWhat) const
{
    switch (eWhat)
    {
        case CEnums::Clone:
        case CEnums::ShowGlobalGraph:
        case CEnums::ShowBranchLog:
        case CEnums::ShowFileLog:
        case CEnums::Commit:
        case CEnums::Pull:
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

void CSvnCommands::onExecFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue)
{
    Q_UNUSED(sPath);

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
    case CEnums::eSetCurrentBranch:
    case CEnums::eCommitReset:
    case CEnums::eCommitRebase:
    case CEnums::eCommitSquash:
    case CEnums::eChangeCommitMessage:
    case CEnums::eContinueRebase:
    case CEnums::eAbortRebase:
    {
        emit newOutputString(eCommand, sValue);
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
        QStringList lReturnValue;
        emit newOutputStringList(eCommand, lReturnValue);
        break;
    }

    case CEnums::eBranchHeadCommits:
    {
        break;
    }

    case CEnums::eAllFileStatus:
    {
        QList<CRepoFile*> lReturnValue;
        emit newOutputListOfCRepoFile(eCommand, lReturnValue);
        break;
    }

    case CEnums::eFileLog:
    case CEnums::eBranchLog:
    {
        QList<CLogLine*> lReturnValue;
        emit newOutputListOfCLogLine(eCommand, lReturnValue);
        break;
    }

    case CEnums::eGraph:
    {
        QList<CGraphLine*> lReturnValue;
        emit newOutputListOfCGraphLine(eCommand, lReturnValue);
        break;
    }

    }
}
