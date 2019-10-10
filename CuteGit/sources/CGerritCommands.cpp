
// Qt
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCoreApplication>

// Application
#include "CGerritCommands.h"
#include "Strings.h"

/*!
    \class CGerritCommands
    \inmodule CuteGit
    \section1 General
    This is the repository control interface for Gerrit, a code review tool based on Git.

    \code


    \endcode
*/

//-------------------------------------------------------------------------------------------------
// Constants

const QString CGerritCommands::sCommandPushGerrit       = "git push origin \"HEAD:refs/for/%1\"";

//-------------------------------------------------------------------------------------------------

CGerritCommands::CGerritCommands(CController* pController)
    : CGitCommands(pController)
{
}

//-------------------------------------------------------------------------------------------------

void CGerritCommands::push(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, Strings::s_sPushingToGerrit);
    QString sCurrentBranch = execNow(sPath, sCommandCurrentBranch).trimmed();
    if (not sCurrentBranch.isEmpty())
    {
        QString sCommand = QString(sCommandPushGerrit).arg(sCurrentBranch);
        exec(new CProcessCommand(CEnums::ePush, sPath, sCommand, true));
    }
    else
    {
        emit newOutputString(CEnums::eNotification, Strings::s_sUnableToGetBranch);
    }
}
