#include <QApplication>
#include "settings.h"
#include "mainwindowclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings settings;
    if (!settings.load(QCoreApplication::applicationDirPath() + "/settings.json"))
        return 1;

    MainWindowClient window(settings);
    window.show();
    return QApplication::exec();
}
