
#pragma once

// Qt
#include <QAbstractListModel>

// qt-plus
#include "CLogLine.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CRepository;

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
        eMessageRole,
        eLabelsRole,
        eMarkedAsDiffFromRole,
        eMarkedAsDiffToRole,
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
    CLogModel(CRepository* pRepository, QObject *parent = nullptr);

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

    //!
    void setCommitMessage(const QString& sCommitId, const QString& sMessage);

    //!
    void commitChanged(const QString& sCommitId);

    //!
    void invalidate();

private:

    //! Lines of the log
    QList<CLogLine*>  m_lLines;
};
