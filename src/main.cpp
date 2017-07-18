// Std import
#include <iostream>

// Qt import 
#include <QApplication>

// App import
#include "markdown_editor.hpp"


int main(int argc, char* argv[]) {

    QApplication* app = new QApplication(argc, argv);

    MarkdownEditor* editor = new MarkdownEditor();

    editor->showMaximized();

    int returnCode =  app->exec();

    delete editor;
    delete app;

    return returnCode;

}
