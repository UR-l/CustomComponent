#include "arcWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArcWidget w;

	w.setBlowUpIndex(2);

    w.show();
    return a.exec();
}
