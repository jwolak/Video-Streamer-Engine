#include "MainWindow.h"
#include <QApplication>
#include "Logger.h"

int main(int argc, char *argv[])
{
    SET_LOG_LEVEL(equinox_logger::LogLevelType::LOG_LEVEL_DEBUG);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
