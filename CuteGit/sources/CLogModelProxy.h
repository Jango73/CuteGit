
#pragma once

// Qt
#include <QSortFilterProxyModel>

// qt-plus
#include "CLogLineCollection.h"

// Application
#include "CLabelModel.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CRepository;

//-------------------------------------------------------------------------------------------------

class CLogModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CRepository*, p, repository, Repository)
    Q_FAST_PROPERTY(QString, s, textFilter, TextFilter)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CLogModelProxy(CRepository* pRepository, QObject *parent = nullptr);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    void filterChanged();

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void requestLogData(int iStartIndex, int iCount);

private:

    //! Lines of the log
    QList<CLogLine*> m_lLines;

    //! Labels for commits
    QMap<QString, CLabelModel*> m_mLabels;
};
