
// Qt
#include <QApplication>
#include <QtSvg>

// Application
#include "CuteGit.h"

// Available arguments
static const char* sSwitch_foo = "--foo";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList lArgList;
    QString sSequenceFile = "";
    bool bMasterMode = true;

    // Check arguments
    if (argc > 1)
    {
        for (int iIndex = 0; iIndex < argc; iIndex++)
            lArgList.append(QString(argv[iIndex]));

        if (lArgList.contains(sSwitch_foo))
        {
            // Do something...
        }
        else
        {
            bMasterMode = false;
            sSequenceFile = lArgList[1];
        }
    }

    // Build CuteGit instance
    CuteGit cuteGit(bMasterMode, sSequenceFile);

    // Event loop
    return app.exec();
}
