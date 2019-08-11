
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
    Q_FAST_PROPERTY(QList<CRepoFile*>, l, repoFiles, RepoFiles)
    Q_FAST_PROPERTY(CHashOfRepoFile, h, hashRepoFiles, HashRepoFiles)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CFlatFileModel(CRepository* pRepository, QObject *parent = nullptr);

    //! Destructor
    virtual ~CFlatFileModel() override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const override;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    //! Returns data
    virtual QVariant data(const QModelIndex& qIndex, int iRole) const override;

    //! Sets data
    virtual bool setData(const QModelIndex& qIndex, const QVariant& vValue, int iRole = Qt::EditRole) override;

    //!
    void handleRepoFilesChanged();

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

public:

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
