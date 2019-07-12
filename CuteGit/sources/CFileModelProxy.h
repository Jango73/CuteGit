
#pragma once

// Qt
#include <QSortFilterProxyModel>

class CFileModelProxy : public QSortFilterProxyModel
{
public:

    CFileModelProxy(QObject* parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private :

    bool hasToBeDisplayed(const QModelIndex index) const;
};
