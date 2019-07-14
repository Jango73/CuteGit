
#pragma once

// Qt
#include <QSortFilterProxyModel>

class CFileModelProxy : public QSortFilterProxyModel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

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

    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE void stageSelection(QModelIndexList lIndices);

    //!
    Q_INVOKABLE void unstageSelection(QModelIndexList lIndices);

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
    bool hasToBeDisplayed(const QModelIndex index) const;
};
