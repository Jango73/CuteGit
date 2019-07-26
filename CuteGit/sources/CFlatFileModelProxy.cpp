
// Qt
#include <QDebug>

// Application
#include "CFlatFileModelProxy.h"
#include "CFlatFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CFlatFileModelProxy::CFlatFileModelProxy(CController* pController, QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_pController(pController)
{
}

//-------------------------------------------------------------------------------------------------

bool CFlatFileModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    CFlatFileModel* pModel = dynamic_cast<CFlatFileModel*>(sourceModel());
    if (pModel != nullptr)
        return statusShown(pModel->data(index, CFlatFileModel::eStatusRole).toString());

    return false;
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModelProxy::filterChanged()
{
    invalidateFilter();
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModelProxy::handleCurrentIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        CFlatFileModel* pModel = dynamic_cast<CFlatFileModel*>(sourceModel());
        if (pModel != nullptr)
            pModel->handleCurrentIndex(mapToSource(qIndex));
    }
}

//-------------------------------------------------------------------------------------------------

QStringList CFlatFileModelProxy::selectionToFullNameList(QModelIndexList lIndices)
{
    QStringList lFullNames;

    CFlatFileModel* pModel = dynamic_cast<CFlatFileModel*>(sourceModel());
    if (pModel != nullptr)
        return pModel->selectionToFullNameList(indexListToSource(lIndices));

    return lFullNames;
}

//-------------------------------------------------------------------------------------------------

QModelIndexList CFlatFileModelProxy::indexListToSource(QModelIndexList lIndices) const
{
    QModelIndexList targetIndices;

    for (QModelIndex qIndex : lIndices)
    {
        if (qIndex.isValid())
        {
            targetIndices << mapToSource(qIndex);
        }
    }

    return targetIndices;
}

//-------------------------------------------------------------------------------------------------

bool CFlatFileModelProxy::statusShown(const QString& sStatus) const
{
    if (m_pController->showClean() && sStatus == CRepoFile::sTokenClean)
        return true;

    if (m_pController->showAdded() && sStatus == CRepoFile::sTokenAdded)
        return true;

    if (m_pController->showModified() && sStatus == CRepoFile::sTokenModified)
        return true;

    if (m_pController->showModified() && sStatus == CRepoFile::sTokenRenamed)
        return true;

    if (m_pController->showDeleted() && sStatus == CRepoFile::sTokenDeleted)
        return true;

    if (m_pController->showUntracked() && sStatus == CRepoFile::sTokenUntracked)
        return true;

    return false;
}
