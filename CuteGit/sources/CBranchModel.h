
#pragma once

// Qt
#include <QAbstractListModel>

// Application
#include "CBranch.h"

//-------------------------------------------------------------------------------------------------

class CBranchModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum ERoles
    {
        eNameRole = Qt::UserRole + 1,
        eCommitIdRole,
    };

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CBranchModel(QObject *parent = nullptr);

    //! Destructor
    virtual ~CBranchModel();

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QStringList labelsForCommit(const QString& sCommitId) const;

    //!
    QStringList branchNames() const;

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
    void setBranchList(QList<CBranch*> lNewList);

    //!
    void setBranchHeadCommit(const QString& sBranch, const QString& sCommitId);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

private:

    //! List of branches
    QList<CBranch*> m_lBranches;
};
