
#pragma once

// Qt
#include <QAbstractListModel>

// qt-plus
#include "CLogLine.h"

//-------------------------------------------------------------------------------------------------

class CLogModel : public QAbstractListModel
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
    CLogModel(QObject *parent = nullptr);

    //! Destructor
    virtual ~CLogModel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Sets all lines
    void setLines(QList<CLogLine*> lNewLines);

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const;

    //! Adds a message
    bool addMessage(qint64 iUserId, QString sMessage);

private:

    //! Chat object
    QList<CLogLine*>  m_lLines;
};
