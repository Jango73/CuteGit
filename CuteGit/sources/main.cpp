
// Qt
#include <QApplication>

// Application
#include "CuteGit.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Build CuteGit instance
    CuteGit cuteGit;

    // Event loop
    return app.exec();
}
