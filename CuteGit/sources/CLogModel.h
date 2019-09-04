
#pragma once

// Qt
#include <QAbstractListModel>

// qt-plus
#include "CLogLineCollection.h"

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
        eFullMessageRole,
        eLabelsRole,
        eMessageIsCompleteRole,
        eMarkedAsDiffFromRole,
        eMarkedAsDiffToRole,
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CRepository*, p, repository, Repository)
    Q_FAST_PROPERTY(int, i, potentialCount, PotentialCount)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CLogModel(CRepository* pRepository, QObject *parent = nullptr);

    //! Destructor
    virtual ~CLogModel() override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Clear all lines
    void clear();

    //! Sets some lines
    void setLines(CLogLineCollection tCollection);

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const override;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const override;

    //!
    virtual bool canFetchMore(const QModelIndex& parent) const override;

    //!
    virtual void fetchMore(const QModelIndex& parent) override;

    //!
    void setCommitMessage(const QString& sCommitId, const QString& sMessage);

    //!
    void commitChanged(const QString& sCommitId);

    //!
    void invalidate();

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void requestLogData(int iStartIndex, int iCount);

private:

    //! Lines of the log
    QList<CLogLine*> m_lLines;
};
