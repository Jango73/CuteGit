
// Qt
#include <QDebug>
#include <QFile>
#include <QTextStream>

// Application
#include "CUtils.h"

//-------------------------------------------------------------------------------------------------

QString CUtils::getTextFileContents(const QString& sFileName)
{
    QFile file(sFileName);
    QString sInputText;

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        sInputText = stream.readAll();
        file.close();
    }

    return sInputText;
}
