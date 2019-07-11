
// Qt
#include <QDebug>

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------

CRepoFile::CRepoFile()
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
