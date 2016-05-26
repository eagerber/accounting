
#include <QApplication>
#include "accounting-mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AccountingMainWindow w;
    w.show();

    return a.exec();
}
