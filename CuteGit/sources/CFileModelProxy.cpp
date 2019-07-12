
// Qt
#include <QDebug>

// Application
#include "CFileModelProxy.h"

//-------------------------------------------------------------------------------------------------

CFileModelProxy::CFileModelProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

bool CFileModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    return hasToBeDisplayed(index);
}

//-------------------------------------------------------------------------------------------------

bool CFileModelProxy::hasToBeDisplayed(const QModelIndex index) const
{
    bool result = false;

    if (sourceModel()->rowCount(index) > 0)
    {
        for (int ii = 0; ii < sourceModel()->rowCount(index); ii++)
        {
            QModelIndex childIndex = sourceModel()->index(ii, 0, index);
            if ( ! childIndex.isValid() )
                break;

            result = hasToBeDisplayed(childIndex);

            if (result)
                break;
        }
    }
    else
    {
        QModelIndex useIndex = sourceModel()->index(index.row(), 1, index.parent());
        QString type = sourceModel()->data(useIndex, Qt::DisplayRole).toString();

        if (! type.contains(filterRegExp()))
        {
            result = false;
        }
        else
        {
            result = true;
        }
    }

    return result;
}
