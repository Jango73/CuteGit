
// Qt
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCoreApplication>

// Application
#include "CGerritCommands.h"

/*!
    \class CGerritCommands
    \inmodule CuteGit
    \section1 General
    This is the repository control interface for Git.

    \code


    \endcode
*/

//-------------------------------------------------------------------------------------------------
// Constants

const QString CGerritCommands::sCommandPushGerrit       = "git push origin \"HEAD:refs/for/%1\"";

//-------------------------------------------------------------------------------------------------

void CGerritCommands::push(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, tr("Pushing to gerrit..."));
    QString sCurrentBranch = execNow(sPath, sCommandCurrentBranch).trimmed();
    if (not sCurrentBranch.isEmpty())
    {
        QString sCommand = QString(sCommandPushGerrit).arg(sCurrentBranch);
        exec(new CProcessCommand(CEnums::ePush, sPath, sCommand, true));
    }
    else
    {
        emit newOutputString(CEnums::eNotification, tr("Unable to get current branch name..."));
    }
}
