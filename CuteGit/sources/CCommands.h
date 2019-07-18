
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------

class CCommands
{
public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CCommands();

    //! Destructor
    virtual ~CCommands();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    QString exec(const QString& sWorkPath, const QString& sCommand);

    //!
    virtual QVector<CRepoFile*> getAllFileStatus(const QString& sPath);

    //!
    virtual QStringList getGraph(const QString& sPath, const QDateTime& from, const QDateTime& to);

    //!
    virtual QString stageFile(const QString& sPath, const QString& sFullName, bool bStage);

    //!
    virtual QString stageAll(const QString& sPath, bool bStage);

    //!
    virtual QString revertFile(const QString& sPath, const QString& sFullName);

    //!
    virtual QString commit(const QString& sPath, const QString& sMessage);

    //!
    virtual QString push(const QString& sPath);

    //!
    virtual QString pull(const QString& sPath);

    //!
    virtual QString unstagedDiff(const QString& sPath, const QString& sFullName);
};
