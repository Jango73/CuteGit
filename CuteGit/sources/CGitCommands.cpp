
// Qt
#include <QDebug>
#include <QRegExp>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

static const char* sCommandStatus = "git status -s";
static const char* sCommandGraph = "git log --graph --decorate --after=\"%1\" --before=\"%2\"";

const QString sStatusAdded = "A";
const QString sStatusModified = "M";
const QString sStatusDeleted = "D";
const QString sStatusUntracked = "??";

//-------------------------------------------------------------------------------------------------

CGitCommands::CGitCommands()
{
}

//-------------------------------------------------------------------------------------------------

CGitCommands::~CGitCommands()
{
}

//-------------------------------------------------------------------------------------------------

QVector<CRepoFile*> CGitCommands::getAllFileStatus(const QString& sPath)
{
    QVector<CRepoFile*> vReturnValue;

    QString sOutput = exec(sPath, sCommandStatus);

    QStringList lStrings = sOutput.split("\n");

    QRegExp tRegExp("\\s*(.*)\\s+(.*)");

    for (QString sLine : lStrings)
    {
        if (tRegExp.indexIn(sLine) != -1)
        {
            QString sStatus = tRegExp.cap(1).trimmed();
            QString sFullName = sPath + "/" + tRegExp.cap(2).trimmed();

            CRepoFile::ERepoFileStatus eStatus = CRepoFile::eClean;

            if (sStatus == sStatusAdded)
                eStatus = CRepoFile::eAdded;
            else if (sStatus == sStatusModified)
                eStatus = CRepoFile::eModified;
            else if (sStatus == sStatusDeleted)
                eStatus = CRepoFile::eDeleted;
            else if (sStatus == sStatusUntracked)
                eStatus = CRepoFile::eUntracked;

            CRepoFile* pFile = new CRepoFile();
            pFile->setStatus(eStatus);
            pFile->setFullName(sFullName);

            vReturnValue << pFile;
        }
    }

    return vReturnValue;
}

//-------------------------------------------------------------------------------------------------

QStringList CGitCommands::getGraph(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    QStringList lReturnValue;

    QString sFrom = from.toString(Qt::ISODate);
    QString sTo = to.toString(Qt::ISODate);

//    sFrom.replace("T", " ");
//    sTo.replace("T", " ");

    QString sCommand = QString(sCommandGraph).arg(sFrom).arg(sTo);
    QString sOutput = exec(sPath, sCommand);

    return sOutput.split("\n");
}
