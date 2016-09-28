#include "mainwindow.h"
#include "controler.h"
#include "logger.hpp"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    GLogger glog(argv[0],"./logs");

    return a.exec();
}
