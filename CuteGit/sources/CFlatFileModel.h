
#pragma once

// Qt
#include <QAbstractListModel>

// Application
#include "CRepoFile.h"
#include "CLogLine.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CRepository;

//-------------------------------------------------------------------------------------------------

class CFlatFileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eFullNameRole = Qt::UserRole + 1,
        eFileNameRole,
        eRelativeNameRole,
        eSizeRole,
        eStatusRole,
        eStagedRole
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CRepository*, p, repository, Repository)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CFlatFileModel(CRepository* pRepository, QObject *parent = nullptr);

    //! Destructor
    virtual ~CFlatFileModel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    //! Returns data
    virtual QVariant data(const QModelIndex& qIndex, int iRole) const;

    //!
    void handleRepoFilesChanged();

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE void handleCurrentIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE QStringList selectionToFullNameList(QModelIndexList lIndices);

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void currentFileFullName(QString sFileFullName);
};
