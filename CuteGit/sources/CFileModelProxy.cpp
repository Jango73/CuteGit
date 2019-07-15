
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
        invalidate();
        emit showCleanChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowAdded(bool bValue)
{
    if (m_bShowAdded != bValue)
    {
        m_bShowAdded = bValue;
        invalidate();
        emit showAddedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowModified(bool bValue)
{
    if (m_bShowModified != bValue)
    {
        m_bShowModified = bValue;
        invalidate();
        emit showModifiedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowDeleted(bool bValue)
{
    if (m_bShowDeleted != bValue)
    {
        m_bShowDeleted = bValue;
        invalidate();
        emit showDeletedChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModelProxy::setShowUntracked(bool bValue)
{
    if (m_bShowUntracked != bValue)
    {
        m_bShowUntracked = bValue;
        invalidate();
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

bool CFileModelProxy::hasToBeDisplayed(const QModelIndex qIndex) const
{
    bool result = false;

    CFileModel* pModel = dynamic_cast<CFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        if (pModel->isDir(qIndex))
            return true;
    }

    QModelIndex qSubIndex = sourceModel()->index(qIndex.row(), 1, qIndex.parent());
    QString sName = sourceModel()->data(qSubIndex, CFileModel::FileNameRole).toString();
    QString sStatus = sourceModel()->data(qSubIndex, CFileModel::eStatusRole).toString();

    if (m_bShowClean && sStatus == CRepoFile::sTokenClean)
        result = true;

    if (m_bShowAdded && sStatus == CRepoFile::sTokenAdded)
        result = true;

    if (m_bShowModified && sStatus == CRepoFile::sTokenModify)
        result = true;

    if (m_bShowDeleted && sStatus == CRepoFile::sTokenDeleted)
        result = true;

    if (m_bShowUntracked && sStatus == CRepoFile::sTokenUntracked)
        result = true;

    return result;
}
