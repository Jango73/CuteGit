
// Qt
#include <QDebug>

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------

CRepoFile::CRepoFile()
    : m_eStatus(eUntracked)
    , m_bStaged(false)
{
}

//-------------------------------------------------------------------------------------------------

CRepoFile::~CRepoFile()
{
}

//-------------------------------------------------------------------------------------------------

QString CRepoFile::statusToString() const
{
    switch (m_eStatus)
    {
    case eClean:
        return "";
    case eAdded:
        return "+";
    case eModified:
        return "*";
    case eDeleted:
        return "-";
    case eUntracked:
        return "?";
    }
}

//-------------------------------------------------------------------------------------------------

QString CRepoFile::stagedToString() const
{
    return m_bStaged ? "O" : "";
}
