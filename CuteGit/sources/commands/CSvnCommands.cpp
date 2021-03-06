
// Qt
#include <QDebug>

// Application
#include "CSvnCommands.h"

/*!
    \class CSvnCommands
    \inmodule CuteGit
    \section1 General
    This is the repository control interface for Apache Subversion (SVN).

         .-"""-.__     ltb
        /      ' o'\
     ,-;  '.  :   _c
    :_."\._ ) ::-"
           ""m "m*/

//-------------------------------------------------------------------------------------------------

CSvnCommands::CSvnCommands(CController* pController)
    : CCommands(pController)
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
    case CEnums::TagOnCommit:
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

void CSvnCommands::changedFileStatus(const QString& sPath)
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

void CSvnCommands::branchLog(const QString& sPath, int iFrom, int iCount)
{
    Q_UNUSED(sPath);
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);
}

//-------------------------------------------------------------------------------------------------

void CSvnCommands::fileLog(const QString& sPath, const QString& sFullName, int iFrom, int iCount)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);
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

void CSvnCommands::onExecFinished(const CProcessResult& tResult)
{
    Q_UNUSED(tResult);
}
