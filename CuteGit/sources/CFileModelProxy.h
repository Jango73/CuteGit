
#pragma once

// Qt
#include <QSortFilterProxyModel>

// qt-plus
#include <Macros.h>

class CFileModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showClean, ShowClean)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showAdded, ShowAdded)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showModified, ShowModified)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showDeleted, ShowDeleted)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showUntracked, ShowUntracked)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CFileModelProxy(QObject* parent = nullptr);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QModelIndex rootPathIndex() const;

    //!
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE QString statusForIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE QString stagedForIndex(QModelIndex qIndex);

    //!
    Q_INVOKABLE void stageSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void unstageSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void revertSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void push();

    //!
    Q_INVOKABLE void handleCurrentIndex(QModelIndex qIndex);

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
