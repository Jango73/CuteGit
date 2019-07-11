
// Qt
#include <QDebug>

// Application
#include "CController.h"
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

CController::CController(QObject *parent)
    : QObject(parent)
    , m_pFileModel(new CFileModel(this, this))
    , m_pCommands(new CGitCommands())
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
    m_pFileModel->setRootPath("D:/Work/Projects/various/AGL32");
}
