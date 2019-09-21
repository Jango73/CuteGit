
#pragma once

// Qt
#include <QObject>

// qt-plus
#include "Macros.h"
#include "CFastList.h"

// Application
#include "CEnums.h"

//-------------------------------------------------------------------------------------------------

class CRepoFile : public QObject
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, fullName, FullName)
    Q_FAST_PROPERTY(QString, s, fileName, FileName)
    Q_FAST_PROPERTY(QString, s, relativeName, RelativeName)
    Q_FAST_PROPERTY(CEnums::ERepoFileStatus, e, status, Status)
    Q_FAST_PROPERTY(bool, b, staged, Staged)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CRepoFile(QObject* parent = nullptr);

    //! Copy constructor
    CRepoFile(const CRepoFile& target, QObject* parent = nullptr);

    //! Destructor
    virtual ~CRepoFile() override;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QString statusToString() const;

    //!
    QString stagedToString() const;

    //-------------------------------------------------------------------------------------------------
    // Operators
    //-------------------------------------------------------------------------------------------------

    bool operator < (const CRepoFile& target);

    //-------------------------------------------------------------------------------------------------
    // Constants
    //-------------------------------------------------------------------------------------------------

    static const QString sTokenClean;
    static const QString sTokenAdded;
    static const QString sTokenModified;
    static const QString sTokenRenamed;
    static const QString sTokenDeleted;
    static const QString sTokenMissing;
    static const QString sTokenUntracked;
    static const QString sTokenIgnored;

    static const QString sTokenStaged;
    static const QString sTokenUnstaged;

    static const QString sRepositoryStatusClean;
    static const QString sRepositoryStatusMerge;
    static const QString sRepositoryStatusRebase;
    static const QString sRepositoryStatusInteractiveRebase;
};

typedef CFastList<QString, CRepoFile*> CRepoFileList;
Q_DECLARE_METATYPE(CRepoFileList)
