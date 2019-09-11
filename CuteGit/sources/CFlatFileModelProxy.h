
#pragma once

// Qt
#include <QSortFilterProxyModel>

// qt-plus
#include "Macros.h"

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CFlatFileModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CController*, p, controller, Controller)
    Q_FAST_PROPERTY(QString, s, nameFilter, NameFilter)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CFlatFileModelProxy(CController* pController, QObject* parent = nullptr);

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

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    QModelIndexList indexListToSource(QModelIndexList lIndices) const;

    //!
    bool statusShown(const QString& sStatus) const;

    //!
    bool nameShown(const QString& sName) const;
};
