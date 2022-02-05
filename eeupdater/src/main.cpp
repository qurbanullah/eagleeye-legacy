#include "include/eeupdater.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EEUpdater w;
    w.show();
    return a.exec();
}
