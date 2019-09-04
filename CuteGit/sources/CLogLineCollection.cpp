
// Application
#include "CLogLineCollection.h"

//-------------------------------------------------------------------------------------------------

CLogLineCollection::CLogLineCollection(QObject* parent)
    : QObject(parent)
    , m_iPotentialCount(0)
    , m_iStartIndex(0)
{
}

//-------------------------------------------------------------------------------------------------

CLogLineCollection::CLogLineCollection(const CLogLineCollection& target)
    : QObject()
    , m_iPotentialCount(target.m_iPotentialCount)
    , m_iStartIndex(target.m_iStartIndex)
    , m_lLines(target.m_lLines)
{
}

//-------------------------------------------------------------------------------------------------

CLogLineCollection::~CLogLineCollection()
{
}

//-------------------------------------------------------------------------------------------------

void CLogLineCollection::add(CLogLine* pLine)
{
    m_lLines << pLine;
}
