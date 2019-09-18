
// Qt
#include <QApplication>
#include <QtSvg>

// Application
#include "CuteGit.h"

// Available arguments
static const char* sSwitch_stub = "--stub";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList lArgList;
    QString sSequenceFile;
    QString sStubFileName;
    bool bMasterMode = true;

    // Check arguments
    if (argc > 1)
    {
        for (int iIndex = 1; iIndex < argc; iIndex++)
        {
            if (QString(argv[iIndex]).contains(sSwitch_stub))
            {
                if (iIndex < argc - 1)
                {
                    iIndex++;
                    sStubFileName = argv[iIndex];
                }
            }
            else
            {
                QFile file(argv[iIndex]);

                if (file.exists())
                {
                    bMasterMode = false;
                    sSequenceFile = argv[iIndex];
                }
            }
        }
    }

    // Build CuteGit instance
    CuteGit cuteGit(bMasterMode, sStubFileName, sSequenceFile);

    // Event loop
    return app.exec();
}
