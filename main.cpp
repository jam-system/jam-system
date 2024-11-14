#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QApplication>


QString readStyleSheet(const QString &path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "File not found";
        return "";
    }

    QTextStream stream(&file);
    QString stylesheet = stream.readAll();
    file.close();
    return stylesheet;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load the stylesheet from a file
    QString stylesheet = readStyleSheet("/Users/jameelberg/Qt_Projects/JamS8_FilmScannerUI_Qt6_Widgets/stylesheet.qss");
    //qDebug() << stylesheet;
    a.setStyleSheet(stylesheet);

    MainWindow w;
    w.show();
    return a.exec();
}
