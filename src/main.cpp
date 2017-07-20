// Std import
#include <iostream>

// Qt import 
#include <QApplication>

// App import
#include "markdown_mainwindow.hpp"


int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    MarkdownEditor editor;

    editor.showMaximized();

    return app.exec();

}
