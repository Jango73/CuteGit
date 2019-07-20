
#pragma once

// Qt
#include <QAbstractListModel>

// qt-plus
#include "CGraphLine.h"

//-------------------------------------------------------------------------------------------------

class CGraphModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eCommitIdRole = Qt::UserRole + 1,
        eDateRole,
        eAuthorRole,
        eMessageRole
    };

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CGraphModel(QObject *parent = nullptr);

    //! Destructor
    virtual ~CGraphModel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Sets all lines
    void setGraphLines(QList<CGraphLine*> lNewLines);

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const;

    //! Adds a message
    bool addMessage(qint64 iUserId, QString sMessage);

    //! Model empty?
    bool isEmpty() const;

private:

    //! Chat object
    QList<CGraphLine*>  m_lGraphLines;
};
