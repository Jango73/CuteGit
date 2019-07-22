
#pragma once

// Qt
#include <QObject>

// qt-plus
#include "Macros.h"

//-------------------------------------------------------------------------------------------------

class CRepoFile : public QObject
{
    Q_OBJECT

public:

    enum ERepoFileStatus
    {
        eClean,
        eAdded,
        eModified,
        eRenamed,
        eDeleted,
        eUntracked,
        eIgnored
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, fullName, FullName)
    Q_FAST_PROPERTY(QString, s, fileName, FileName)
    Q_FAST_PROPERTY(QString, s, relativeName, RelativeName)
    Q_FAST_PROPERTY(ERepoFileStatus, e, status, Status)
    Q_FAST_PROPERTY(bool, b, staged, Staged)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CRepoFile(QObject* parent = nullptr);

    //! Destructor
    virtual ~CRepoFile() override;

    //!
    QString statusToString() const;

    //!
    QString stagedToString() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Constants
    //-------------------------------------------------------------------------------------------------

    static const QString sTokenClean;
    static const QString sTokenAdded;
    static const QString sTokenModified;
    static const QString sTokenRenamed;
    static const QString sTokenDeleted;
    static const QString sTokenUntracked;
    static const QString sTokenIgnored;
    static const QString sTokenStaged;
    static const QString sTokenUnstaged;

    static const QString sRepositoryStatusClean;
    static const QString sRepositoryStatusMerge;
    static const QString sRepositoryStatusRebase;
    static const QString sRepositoryStatusInteractiveRebase;
};
