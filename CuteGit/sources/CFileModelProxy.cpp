
// Qt
#include <QDebug>

// Application
#include "CFileModelProxy.h"
#include "CFileModel.h"

//-------------------------------------------------------------------------------------------------

CFileModelProxy::CFileModelProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_bShowClean(false)
    , m_bShowAdded(true)
    , m_bShowModified(true)
    , m_bShowDeleted(true)
    , m_bShowUntracked(false)
{
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowClean(bool bValue)
{
    if (m_bShowClean != bValue)
    {
        m_bShowClean = bValue;
        invalidateFilter();
        emit showCleanChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowAdded(bool bValue)
{
    if (m_bShowAdded != bValue)
    {
        m_bShowAdded = bValue;
        invalidateFilter();
        emit showAddedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowModified(bool bValue)
{
    if (m_bShowModified != bValue)
    {
        m_bShowModified = bValue;
        invalidateFilter();
        emit showModifiedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowDeleted(bool bValue)
{
    if (m_bShowDeleted != bValue)
    {
        m_bShowDeleted = bValue;
        invalidateFilter();
        emit showDeletedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowUntracked(bool bValue)
{
    if (m_bShowUntracked != bValue)
    {
        m_bShowUntracked = bValue;
        invalidateFilter();
        emit showUntrackedChanged();
    }
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

    return hasToBeDisplayed(index);
}

//-------------------------------------------------------------------------------------------------

QString CFileModelProxy::statusForIndex(QModelIndex qIndex)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        return pModel->data(mapToSource(qIndex), CFileModel::eStatusRole).toString();
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

QString CFileModelProxy::stagedForIndex(QModelIndex qIndex)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        return pModel->data(mapToSource(qIndex), CFileModel::eStagedRole).toString();
    }

    return "";
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

void CFileModelProxy::stageAll()
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->stageAll();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::revertSelection(QModelIndexList lIndices)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->revertSelection(indexListToSource(lIndices));
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::commit(const QString& sMessage)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->commit(sMessage);
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::push()
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->push();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::pull()
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->pull();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::handleCurrentIndex(QModelIndex qIndex)
{
    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        pModel->handleCurrentIndex(mapToSource(qIndex));
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

bool CFileModelProxy::hasToBeDisplayed(const QModelIndex qIndex) const
{
    QModelIndex qSubIndex = sourceModel()->index(qIndex.row(), 1, qIndex.parent());
    QString sName = sourceModel()->data(qSubIndex, CFileModel::FileNameRole).toString();
    QString sStatus = sourceModel()->data(qSubIndex, CFileModel::eStatusRole).toString();

    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        if (pModel->isDir(qSubIndex))
        {
            if (sStatus == CRepoFile::sTokenIgnored)
                return false;

            return true;
        }
    }

    return statusShown(sStatus);
}


//-------------------------------------------------------------------------------------------------

bool CFileModelProxy::statusShown(const QString& sStatus) const
{
    if (m_bShowClean && sStatus == CRepoFile::sTokenClean)
        return true;

    if (m_bShowAdded && sStatus == CRepoFile::sTokenAdded)
        return true;

    if (m_bShowModified && sStatus == CRepoFile::sTokenModified)
        return true;

    if (m_bShowDeleted && sStatus == CRepoFile::sTokenDeleted)
        return true;

    if (m_bShowUntracked && sStatus == CRepoFile::sTokenUntracked)
        return true;

    return false;
}
