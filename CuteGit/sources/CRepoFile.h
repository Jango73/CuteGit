
#pragma once

// Qt
#include <QObject>

// qt-plus
#include <Macros.h>

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
        eDeleted,
        eUntracked
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, fullName, FullName)
    Q_FAST_PROPERTY(ERepoFileStatus, e, status, Status)
    Q_FAST_PROPERTY(bool, b, staged, Staged)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CRepoFile();

    //! Destructor
    virtual ~CRepoFile();

    //!
    QString statusToString() const;

    //!
    QString stagedToString() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Coonstants
    //-------------------------------------------------------------------------------------------------

    static const QString sTokenClean;
    static const QString sTokenAdded;
    static const QString sTokenModify;
    static const QString sTokenDeleted;
    static const QString sTokenUntracked;
    static const QString sTokenStaged;
    static const QString sTokenUnstaged;
};
