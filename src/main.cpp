// Std import
#include <iostream>

// Qt import 
#include <QApplication>
#include <QFile>

// App import
#include "markdown_mainwindow.hpp"


int main(int argc, char* argv[]) {

    QFile style(":/style/qt-dark.qss");

    QApplication app(argc, argv);

    if( style.open(QIODevice::ReadOnly | QIODevice::Text) )
        app.setStyleSheet(style.readAll());

    style.close();

    MarkdownEditor editor;

    editor.showMaximized();

    return app.exec();
}
