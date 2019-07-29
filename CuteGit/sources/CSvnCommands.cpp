
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
    case CEnums::BranchFromCommit:
        return true;

    default:
        return false;
    }
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::repositoryStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::allFileStatus(const QString& sPath)
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

void CSvnCommands::onExecFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData)
{
    Q_UNUSED(sPath);
    Q_UNUSED(eCommand);
    Q_UNUSED(sValue);
    Q_UNUSED(sUserData);
}
