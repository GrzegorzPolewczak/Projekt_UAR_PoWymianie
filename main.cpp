#include "ukladautomatycznejregulacji.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./programicon.png"));
    UkladAutomatycznejRegulacji w;
    w.setWindowTitle("Układ Automatycznej Regulacji");
    w.setWindowIcon(QIcon("./programicon.png"));
    w.show();
    return a.exec();
}
