
#pragma once

// Qt
#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QThreadPool>
#include <QRunnable>

// Application
#include "CEnums.h"
#include "CBranch.h"
#include "CRepoFile.h"
#include "CLogLineCollection.h"
#include "CDiffLine.h"
#include "CGraphLine.h"

//-------------------------------------------------------------------------------------------------

class CProcessCommand : public QObject
{
    Q_OBJECT

public:

    CProcessCommand(
            CEnums::EProcessCommand eCommand,
            QString sWorkPath,
            QString sCommand,
            bool bAllowStack = false,
            QMap<QString, QString> mEnvironment = QMap<QString, QString>(),
            QString sUserData = "",
            CEnums::EProcessCommand eEndSignal = CEnums::eNothing
            )
        : m_eCommand(eCommand)
        , m_eEndSignal(eEndSignal)
        , m_bAllowStack(bAllowStack)
        , m_sWorkPath(sWorkPath)
        , m_sCommand(sCommand)
        , m_mEnvironment(mEnvironment)
        , m_sUserData(sUserData)
    {
    }

    CEnums::EProcessCommand m_eCommand;
    CEnums::EProcessCommand m_eEndSignal;
    bool                    m_bAllowStack;
    QString                 m_sWorkPath;
    QString                 m_sCommand;
    QMap<QString, QString>  m_mEnvironment;
    QString                 m_sUserData;
};

//-------------------------------------------------------------------------------------------------

class CExecution : public QThread
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CExecution();

    //! Destructor
    virtual ~CExecution() override;

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    void exec(CProcessCommand* pCommand);

    //!
    QString execNow(QString sWorkPath,
                    QString sCommand,
                    QMap<QString, QString> mEnvironment = QMap<QString, QString>()
                    );

    //!
    QString execNowLiveFeed(
            CEnums::EProcessCommand eCommand,
            QString sWorkPath,
            QString sCommand,
            QMap<QString, QString> mEnvironment = QMap<QString, QString>()
            );

    //-------------------------------------------------------------------------------------------------
    // Private control methods
    //-------------------------------------------------------------------------------------------------

private:

    //!
    virtual void run() override;

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void execFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData);

    //!
    void newOutputString(CEnums::EProcessCommand eCommand, QString sValue);

    //!
    void newOutputKeyValue(CEnums::EProcessCommand eCommand, QString sKey, QString sValue);

    //!
    void newOutputStringList(CEnums::EProcessCommand eCommand, QStringList lValue);

    //!
    void newOutputListOfCBranch(CEnums::EProcessCommand eCommand, QList<CBranch*> lNewBranches);

    //!
    void newOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, CRepoFileList lNewRepoFiles);

    //!
    void newOutputCLogLineCollection(CEnums::EProcessCommand eCommand, CLogLineCollection lNewLines);

    //!
    void newOutputListOfCDiffLine(CEnums::EProcessCommand eCommand, QList<CDiffLine*> lNewLines);

    //!
    void newOutputListOfCGraphLine(CEnums::EProcessCommand eCommand, QList<CGraphLine*> lNewLines);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QThreadPool                 m_tPool;

private:

    bool                        m_bStop;
    QMutex                      m_mMutex;
    CEnums::EProcessCommand     m_eLastShownCommand;
    QList<CProcessCommand*>     m_lCommandStack;
};

//-------------------------------------------------------------------------------------------------

class CCleanFileLister : public QObject, public QRunnable
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    virtual void run() override;

    //!
    void getAllFiles(CRepoFileList& lFileList, const QString& sRootPath, const QString& sCurrentPath);

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void newOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, CRepoFileList lNewRepoFiles);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

public:

    CEnums::EProcessCommand     m_eCommand;
    QString                     m_sRootPath;
};
