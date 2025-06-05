#include <QApplication>
#include "processor_visualizer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ProcessorVisualizer w;
    w.show();
    return a.exec();
}
