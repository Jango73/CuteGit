
// Qt
#include <QDebug>

// Application
#include "CFileModelProxy.h"
#include "CFileModel.h"

//-------------------------------------------------------------------------------------------------

CFileModelProxy::CFileModelProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

QModelIndex CFileModelProxy::rootPathIndex() const
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        return mapFromSource(pModel->rootPathIndex());
    }

    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

bool CFileModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    if (hasToBeDisplayed(index))
        return true;

    return false;
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::stageSelection(QModelIndexList lIndices)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->stageSelection(indexListToSource(lIndices));
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::unstageSelection(QModelIndexList lIndices)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->unstageSelection(indexListToSource(lIndices));
    }
}

//-------------------------------------------------------------------------------------------------

QModelIndexList CFileModelProxy::indexListToSource(QModelIndexList lIndices) const
{
    QModelIndexList targetIndices;

    for (QModelIndex qIndex : lIndices)
    {
        targetIndices << mapToSource(qIndex);
    }

    return targetIndices;
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
            if (! childIndex.isValid())
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
