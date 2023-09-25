#include "bd.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bd w;
    w.show();
    return a.exec();
}
