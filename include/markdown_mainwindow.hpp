#ifndef MARKDOWN_EDITOR_HPP
#define MARKDOWN_EDITOR_HPP

#include <QMainWindow>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QWebEngineView>
#include <QCloseEvent>

#include "markdown_highlighter.hpp"
#include "markdown_textedit.hpp"
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
    QPlainTextEdit*      mTextEdit;
    MarkdownTabWidget*   mTabWidget;
    MarkdownHighlighter* mHighlighter;
    QWebEngineView*      mWebEngine;
    MarkdownDocument*    mDocument;

    void createMenus();

    void setupConnections();

    void setupUI();




private slots:

    void addTab();

    void addTab(MarkdownTextEdit* newEditor);

    void closeTab(int index = -1);

    void exportAsPDF();

    void openFile();

    void refreshDocument();

    void saveAll();

    void saveAs();

    void saveCurrent();

    void swapDocument(int index);

    void updateTab(bool);
};

#endif // MARKDOWN_EDITOR_H
