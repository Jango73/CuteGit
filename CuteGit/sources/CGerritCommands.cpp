
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

const QString CGerritCommands::sCommandPushGerritReady  = "git push origin \"HEAD:refs/for/%1%ready\"";
const QString CGerritCommands::sCommandPushGerritWIP    = "git push origin \"HEAD:refs/for/%1%wip\"";

//-------------------------------------------------------------------------------------------------

CGerritCommands::CGerritCommands(CController* pController)
    : CGitCommands(pController)
{
}

//-------------------------------------------------------------------------------------------------

bool CGerritCommands::can(CEnums::ECapability eWhat) const
{
    Q_UNUSED(eWhat);

    return true;
}

//-------------------------------------------------------------------------------------------------

void CGerritCommands::push(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, Strings::s_sPushingToGerrit);
    QString sCurrentBranch = execNow(sPath, sCommandCurrentBranch).trimmed();
    if (not sCurrentBranch.isEmpty())
    {
        QString sCommand = QString(sCommandPushGerritReady).arg(sCurrentBranch);
        exec(new CProcessCommand(CEnums::ePush, sPath, sCommand, true));
    }
    else
    {
        emit newOutputString(CEnums::eNotification, Strings::s_sUnableToGetBranch);
    }
}

//-------------------------------------------------------------------------------------------------

void CGerritCommands::pushAsWIP(const QString& sPath)
{
    emit newOutputString(CEnums::eNotification, Strings::s_sPushingToGerrit);
    QString sCurrentBranch = execNow(sPath, sCommandCurrentBranch).trimmed();
    if (not sCurrentBranch.isEmpty())
    {
        QString sCommand = QString(sCommandPushGerritWIP).arg(sCurrentBranch);
        exec(new CProcessCommand(CEnums::ePushAsWIP, sPath, sCommand, true));
    }
    else
    {
        emit newOutputString(CEnums::eNotification, Strings::s_sUnableToGetBranch);
    }
}
