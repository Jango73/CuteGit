
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

    Q_FAST_PROPERTY(ERepoFileStatus, e, status, Status)
    Q_FAST_PROPERTY(QString, s, fullName, FullName)

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

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------
};
