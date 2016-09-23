#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    cls_MainWindow w;
    w.show();

    if (argc > 1) {
        w.SetInputFilename(argv[1]);
    }

    if (argc > 2) {
        w.SetOutputFilename(argv[2]);
        w.ImportAnalysisRes();
        w.ExportCorrections();

        return 0;
    }

    return app.exec();
}
