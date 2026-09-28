#include <QApplication>
#include "settings.h"
#include "mainwindowserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings settings;
    if(!settings.load(QCoreApplication::applicationDirPath() + "/settings.json"))
        return 1;

    MainWindowServer window(settings);
    window.show();
    return QApplication::exec();
}
