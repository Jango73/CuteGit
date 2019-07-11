
// Qt
#include <QDebug>
#include <QRegExp>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

const QString sCommandStatus = "git status -s";

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

            qDebug() << sStatus;
            qDebug() << sFullName;

            vReturnValue << pFile;
        }
    }

    return vReturnValue;
}
