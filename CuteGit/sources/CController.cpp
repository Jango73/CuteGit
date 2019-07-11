
// Qt
#include <QDebug>

// Application
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pFileModel(new CFileModel(parent))
{
    loadRepositories();
}

//-------------------------------------------------------------------------------------------------

CController::~CController()
{
}

//-------------------------------------------------------------------------------------------------

void CController::loadRepositories()
{
}
