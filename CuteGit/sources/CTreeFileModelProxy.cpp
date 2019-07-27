
// Qt
#include <QDebug>

// Application
#include "CTreeFileModelProxy.h"
#include "CTreeFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CTreeFileModelProxy::CTreeFileModelProxy(CController* pController, QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_pController(pController)
{
}

//-------------------------------------------------------------------------------------------------

QModelIndex CTreeFileModelProxy::rootPathIndex() const
{
    CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());

    if (pModel != nullptr)
        return mapFromSource(pModel->rootPathIndex());

    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

bool CTreeFileModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return hasToBeDisplayed(index);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModelProxy::filterChanged()
{
    invalidateFilter();
}

//-------------------------------------------------------------------------------------------------

QString CTreeFileModelProxy::statusForIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());
        if (pModel != nullptr)
            return pModel->data(mapToSource(qIndex), CTreeFileModel::eStatusRole).toString();
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

QString CTreeFileModelProxy::stagedForIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());
        if (pModel != nullptr)
            return pModel->data(mapToSource(qIndex), CTreeFileModel::eStagedRole).toString();
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

QString CTreeFileModelProxy::fullNameForIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        QModelIndex qSourceIndex = mapToSource(qIndex);

        if (qSourceIndex.isValid())
        {
            CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());

            if (pModel != nullptr)
            {
                QString sPath = pModel->data(qSourceIndex, CTreeFileModel::FilePathRole).toString();
                QString sName = pModel->data(qSourceIndex, CTreeFileModel::FileNameRole).toString();

                return QString("%1/%2").arg(sPath).arg(sName);
            }
        }
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModelProxy::handleCurrentIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());
        if (pModel != nullptr)
            pModel->handleCurrentIndex(mapToSource(qIndex));
    }
}

//-------------------------------------------------------------------------------------------------

QStringList CTreeFileModelProxy::selectionToFullNameList(QModelIndexList lIndices)
{
    QStringList lFullNames;

    CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());
    if (pModel != nullptr)
        return pModel->selectionToFullNameList(indexListToSource(lIndices));

    return lFullNames;
}

//-------------------------------------------------------------------------------------------------

QModelIndexList CTreeFileModelProxy::indexListToSource(QModelIndexList lIndices) const
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

bool CTreeFileModelProxy::hasToBeDisplayed(const QModelIndex qIndex) const
{
    QModelIndex qSubIndex = sourceModel()->index(qIndex.row(), 1, qIndex.parent());
    QString sName = sourceModel()->data(qSubIndex, CTreeFileModel::FileNameRole).toString();
    QString sStatus = sourceModel()->data(qSubIndex, CTreeFileModel::eStatusRole).toString();

    CTreeFileModel* pModel = dynamic_cast<CTreeFileModel*>(sourceModel());

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

bool CTreeFileModelProxy::statusShown(const QString& sStatus) const
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
