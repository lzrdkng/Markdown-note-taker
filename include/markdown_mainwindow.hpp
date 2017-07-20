#ifndef MARKDOWN_EDITOR_HPP
#define MARKDOWN_EDITOR_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabBar>
#include <QTextBrowser>
#include <QCloseEvent>

#include "markdown_highlighter.hpp"
#include "markdown_textedit.hpp"
#include "markdown_webengine.hpp"
#include "markdown_tabwidget.hpp"
#include "markdown_document.hpp"

class MarkdownEditor : public QMainWindow {

    Q_OBJECT

public:

    explicit MarkdownEditor(QWidget* parent = nullptr);



protected:

    void closeEvent(QCloseEvent* event);

private:

    // MEMBERS
    QSplitter*           mSplitter;
    MarkdownTabWidget*   mTabWidget;
    MarkdownHighlighter* mHighlighter;
    MarkdownWebEngine*   mWebEngine;
    MarkdownDocument*     mDocument;

    void setupActions();

    void setupConnections();

    void setupUI();




public slots:

    void addTab();

    void addTab(MarkdownTextEdit* newEditor);

    void closeTab(int index = -1);

    void openFile();

    void refreshDocument(int index = -1);

    void refreshTabColor();

    void saveAll();

    void saveAs();

    void saveCurrent();

    void swapDocument(int index);

    void updateTab(bool);
};

#endif // MARKDOWN_EDITOR_H
