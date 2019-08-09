
// Qt
#include <QDebug>

// Application
#include "CStagedFileModelProxy.h"
#include "CFlatFileModel.h"

//-------------------------------------------------------------------------------------------------

CStagedFileModelProxy::CStagedFileModelProxy(CController* pController, QObject* parent)
    : CFlatFileModelProxy(pController, parent)
{
}

//-------------------------------------------------------------------------------------------------

bool CStagedFileModelProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    CFlatFileModel* pModel = dynamic_cast<CFlatFileModel*>(sourceModel());

    if (pModel != nullptr)
    {
        QString sStatus = pModel->data(index, CFlatFileModel::eStatusRole).toString();
        QString sStaged = pModel->data(index, CFlatFileModel::eStagedRole).toString();

        return sStatus != CRepoFile::sTokenIgnored && sStaged == CRepoFile::sTokenStaged;
    }

    return false;
}
