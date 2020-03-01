
#pragma once

// Qt
#include <QString>

//-------------------------------------------------------------------------------------------------

class CUtils
{
public:

    //! Returns the contents of a text file
    static QString getTextFileContents(const QString& sFileName);

    //! Writes text to a file
    static void setTextFileContents(const QString& sFileName, const QString& sText);
};
