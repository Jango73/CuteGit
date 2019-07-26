
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
        eMessageRole,
        eGraphSymbolRole,
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
    void setLines(QList<CGraphLine*> lNewLines);

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const;

private:

    //! Lines of the graph
    QList<CGraphLine*>  m_lLines;
};
