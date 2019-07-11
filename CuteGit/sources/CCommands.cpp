
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
