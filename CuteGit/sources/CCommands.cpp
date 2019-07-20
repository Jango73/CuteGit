
// Qt
#include <QDebug>
#include <QProcess>
#include <QMutexLocker>

// Application
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

CCommands::CCommands()
    : m_bStop(false)
    , m_mMutex(QMutex::Recursive)
{
    start();
}

//-------------------------------------------------------------------------------------------------

CCommands::~CCommands()
{
    m_bStop = true;
    wait();
}

//-------------------------------------------------------------------------------------------------

void CCommands::run()
{
    while (not m_bStop)
    {
        CProcessCommand* pCommand = nullptr;

        {
            QMutexLocker locker(&m_mMutex);

            if (m_lCommands.count() > 0)
            {
                pCommand = m_lCommands[0];
                m_lCommands.removeAt(0);
            }
        }

        if (pCommand != nullptr)
        {
            QProcess process;

            if (not pCommand->m_sWorkPath.isEmpty())
                process.setWorkingDirectory(pCommand->m_sWorkPath);

            process.start(pCommand->m_sCommand);
            process.waitForFinished();

            QString sOutput = process.readAllStandardOutput();
            sOutput += process.readAllStandardError();

            emit execFinished(pCommand->m_sWorkPath, pCommand->m_eCommand, sOutput);

            pCommand->deleteLater();
        }

        msleep(100);
    }
}

//-------------------------------------------------------------------------------------------------

void CCommands::exec(CProcessCommand* pCommand)
{
    QMutexLocker locker(&m_mMutex);

    // Use only latest command of a given type
    for (int index = 0; index < m_lCommands.count(); index++)
    {
        if (m_lCommands[index]->m_eCommand == pCommand->m_eCommand)
        {
            m_lCommands[index]->deleteLater();
            m_lCommands.removeAt(index);
            index--;
        }
    }

    m_lCommands << pCommand;
}

//-------------------------------------------------------------------------------------------------

QString CCommands::execNow(QString m_sWorkPath, QString m_sCommand)
{
    QProcess process;

    if (not m_sWorkPath.isEmpty())
        process.setWorkingDirectory(m_sWorkPath);

    process.start(m_sCommand);
    process.waitForFinished();

    QString sOutput = process.readAllStandardOutput();
    sOutput += process.readAllStandardError();

    return sOutput;
}

//-------------------------------------------------------------------------------------------------

void CCommands::allFileStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::repositoryStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branches(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::graph(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    Q_UNUSED(sPath);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

//-------------------------------------------------------------------------------------------------

void CCommands::fileLog(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stageAll(const QString& sPath, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commit(const QString& sPath, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::amend(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::push(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::pull(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::setCurrentBranch(const QString& sPath, const QString& sBranch)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sBranch);
}
