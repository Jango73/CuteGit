
#pragma once

// Qt
#include <QSortFilterProxyModel>

// qt-plus
#include "Macros.h"

// Application
#include "CFlatFileModelProxy.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CStagedFileModelProxy : public CFlatFileModelProxy
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CStagedFileModelProxy(CController* pController, QObject* parent = nullptr);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;
};
