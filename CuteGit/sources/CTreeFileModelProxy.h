
#pragma once

// Qt
#include <QSortFilterProxyModel>

// qt-plus
#include "Macros.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CTreeFileModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CController*, p, controller, Controller)
    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CTreeFileModelProxy(CController* pController, QObject* parent = nullptr);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QModelIndex rootPathIndex() const;

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
    Q_INVOKABLE QString statusForIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE QString stagedForIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE void refresh();

    //!
    Q_INVOKABLE void stageSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void unstageSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void stageAll();

    //!
    Q_INVOKABLE void revertSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void commit(const QString& sMessage, bool bAmend);

    //!
    Q_INVOKABLE void continueRebase();

    //!
    Q_INVOKABLE void push();

    //!
    Q_INVOKABLE void pull();

    //!
    Q_INVOKABLE void handleCurrentIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE void commitRebase(const QString& sCommitId);

    //!
    Q_INVOKABLE void changeCommitMessage(const QString& sCommitId, const QString& sMessage);

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void rootPathIndexChanged();

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    QModelIndexList indexListToSource(QModelIndexList lIndices) const;

    //!
    bool hasToBeDisplayed(const QModelIndex qIndex) const;

    //!
    bool statusShown(const QString& sStatus) const;
};
