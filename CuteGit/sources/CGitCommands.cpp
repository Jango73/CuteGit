
// Qt
#include <QDebug>
#include <QRegExp>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

static const char* sCommandStatus = "git status -s";
// static const char* sCommandGraph = "git log --graph --pretty=format:\"%h | %s | %an | %ai\" --after=\"%1\" --before=\"%2\"";
static const char* sCommandGraph = "git log --graph --pretty=format:\"%h | %s | %an | %ai\" --max-count=20";
static const char* sCommandStage = "git add %1";
static const char* sCommandUnstage = "git reset %1";

const QString sStatusAdded = "A";
const QString sStatusModified = "M";
const QString sStatusDeleted = "D";
const QString sStatusUntracked = "?";

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

    QRegExp tRegExp("([a-zA-Z?\\s])([a-zA-Z?\\s])\\s(.*)");

    for (QString sLine : lStrings)
    {
        if (tRegExp.indexIn(sLine) != -1)
        {
            QString sStaged = tRegExp.cap(1).trimmed();
            QString sUnstaged = tRegExp.cap(2).trimmed();
            QString sFullName = sPath + "/" + tRegExp.cap(3).trimmed();
            bool bStaged = false;

            CRepoFile::ERepoFileStatus eStatus = CRepoFile::eClean;

            if (sStaged.isEmpty() == false)
            {
                bStaged = true;

                if (sStaged == sStatusAdded)
                    eStatus = CRepoFile::eAdded;
                else if (sStaged == sStatusModified)
                    eStatus = CRepoFile::eModified;
                else if (sStaged == sStatusDeleted)
                    eStatus = CRepoFile::eDeleted;
                else if (sStaged == sStatusUntracked)
                {
                    bStaged = false;
                    eStatus = CRepoFile::eUntracked;
                }
            }

            if (sUnstaged.isEmpty() == false)
            {
                if (sUnstaged == sStatusAdded)
                    eStatus = CRepoFile::eAdded;
                else if (sUnstaged == sStatusModified)
                    eStatus = CRepoFile::eModified;
                else if (sUnstaged == sStatusDeleted)
                    eStatus = CRepoFile::eDeleted;
                else if (sUnstaged == sStatusUntracked)
                {
                    bStaged = false;
                    eStatus = CRepoFile::eUntracked;
                }
            }

            CRepoFile* pFile = new CRepoFile();
            pFile->setStatus(eStatus);
            pFile->setFullName(sFullName);
            pFile->setStaged(bStaged);

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

    QString sCommand = QString(sCommandGraph).arg(sFrom).arg(sTo);
    QString sOutput = exec(sPath, sCommand);

    lReturnValue = sOutput.split("\n");

    return lReturnValue;
}

//-------------------------------------------------------------------------------------------------

QString CGitCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    QString sCommand = QString(bStage ? sCommandStage : sCommandUnstage).arg(sFullName);
    QString sOutput = exec(sPath, sCommand);

    return sOutput;
}
