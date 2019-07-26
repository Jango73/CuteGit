
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

bool CCommands::can(ECapability eWhat) const
{
    Q_UNUSED(eWhat);
    return false;
}

//-------------------------------------------------------------------------------------------------

// This method does NOT run in the main thread
// Watch out for data integrity protection
void CCommands::run()
{
    while (not m_bStop)
    {
        CProcessCommand* pCommand = nullptr;

        {
            QMutexLocker locker(&m_mMutex);

            if (m_lCommandStack.count() > 0)
            {
                pCommand = m_lCommandStack[0];
                m_lCommandStack.removeAt(0);
            }
        }

        if (pCommand != nullptr)
        {
            QString sOutput = execNow(pCommand->m_sWorkPath, pCommand->m_sCommand, pCommand->m_mEnvironment);

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

    // Leave only one command of a given type in the stack
    // if not allowed to stack the type
    if (pCommand->m_bAllowStack == false)
    {
        for (int index = 0; index < m_lCommandStack.count(); index++)
        {
            if (m_lCommandStack[index]->m_eCommand == pCommand->m_eCommand)
            {
                m_lCommandStack[index]->deleteLater();
                m_lCommandStack.removeAt(index);
                index--;
            }
        }
    }

    m_lCommandStack << pCommand;
}

//-------------------------------------------------------------------------------------------------

QString CCommands::execNow(QString m_sWorkPath, QString m_sCommand, QMap<QString, QString> m_mEnvironment)
{
    QProcess process;

    if (not m_sWorkPath.isEmpty())
        process.setWorkingDirectory(m_sWorkPath);

    if (not m_mEnvironment.isEmpty())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        for (QString sKey : m_mEnvironment.keys())
            env.insert(sKey, m_mEnvironment[sKey]);

        process.setProcessEnvironment(env);
    }

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

void CCommands::graph(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to)
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

void CCommands::toggleStaged(const QString& sPath, const QString& sFullName)
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

//-------------------------------------------------------------------------------------------------

void CCommands::commitReset(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitRebase(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitSquash(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::continueRebase(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::abortRebase(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::editSequenceFile(const QString& sFileName)
{
    Q_UNUSED(sFileName);
}
