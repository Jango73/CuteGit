
#pragma once

// Qt
#include <QAbstractListModel>

// Application
#include "CLabel.h"

//-------------------------------------------------------------------------------------------------

class CLabelModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum ERoles
    {
        eNameRole = Qt::UserRole + 1,
        eTypeRole,
    };

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CLabelModel(QObject *parent = nullptr);

    //! Destructor
    virtual ~CLabelModel();

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
    void setLabelList(QList<CLabel*> lNewList);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

private:

    //! List of labels
    QList<CLabel*> m_lLabels;
};
