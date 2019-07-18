
// Qt
#include <QDebug>
#include <QProcess>

// Application
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

CCommands::CCommands()
{
}

//-------------------------------------------------------------------------------------------------

CCommands::~CCommands()
{
}

//-------------------------------------------------------------------------------------------------

QString CCommands::exec(const QString& sWorkPath, const QString& sCommand)
{
    QProcess process;

    if (sWorkPath.isEmpty() == false)
        process.setWorkingDirectory(sWorkPath);

    process.start(sCommand);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    output += process.readAllStandardError();

    return output;
}

//-------------------------------------------------------------------------------------------------

QVector<CRepoFile*> CCommands::getAllFileStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
    return QVector<CRepoFile*>();
}

//-------------------------------------------------------------------------------------------------

QStringList CCommands::getBranches(const QString& sPath)
{
    Q_UNUSED(sPath);
    return QStringList();
}

//-------------------------------------------------------------------------------------------------

QStringList CCommands::getGraph(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    Q_UNUSED(sPath);
    Q_UNUSED(from);
    Q_UNUSED(to);
    return QStringList();
}

//-------------------------------------------------------------------------------------------------

QString CCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(bStage);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::stageAll(const QString& sPath, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(bStage);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::commit(const QString& sPath, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sMessage);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::push(const QString& sPath)
{
    Q_UNUSED(sPath);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::pull(const QString& sPath)
{
    Q_UNUSED(sPath);
    return "";
}

//-------------------------------------------------------------------------------------------------

QString CCommands::unstagedDiff(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    return "";
}
