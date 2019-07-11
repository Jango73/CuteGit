
// Qt
#include <QDebug>

// Application
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pFileModel(new CFileModel(parent))
{
}

//-------------------------------------------------------------------------------------------------

CController::~CController()
{
}

//-------------------------------------------------------------------------------------------------

CFileModel* CController::fileModel() const
{
    return m_pFileModel;
}
