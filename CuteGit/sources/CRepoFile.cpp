
// Qt
#include <QDebug>

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------

const QString CRepoFile::sTokenClean = " ";
const QString CRepoFile::sTokenAdded = "+";
const QString CRepoFile::sTokenModified = "*";
const QString CRepoFile::sTokenDeleted = "-";
const QString CRepoFile::sTokenUntracked = "?";
const QString CRepoFile::sTokenStaged = "O";
const QString CRepoFile::sTokenUnstaged = " ";

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
        return sTokenClean;
    case eAdded:
        return sTokenAdded;
    case eModified:
        return sTokenModified;
    case eDeleted:
        return sTokenDeleted;
    case eUntracked:
        return sTokenUntracked;
    }

    return sTokenUntracked;
}

//-------------------------------------------------------------------------------------------------

QString CRepoFile::stagedToString() const
{
    return m_bStaged ? sTokenStaged : sTokenUnstaged;
}
