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



class MarkdownEditor : public QMainWindow {

    Q_OBJECT

public:

    MarkdownEditor();

protected:

    void closeEvent(QCloseEvent* event);

private:

    // MEMBERS
    QTextBrowser*   mTextBrowser;
    QSplitter*      mSplitter;
    QStackedWidget* mStackedWidget;
    QTabBar*        mTabBar;
    MarkdownHighlighter* mHighlighter;

    void removeWidget(int);

public slots:

    void addTab();
    void addWidget(MarkdownTextEdit* newWidget = nullptr);
    void closeTab(int index = -1);
    void openFile();
    void markdownToHtml();
    void refreshHtml(int index = -1);
    void refreshTabColor();
    void saveAll();
    void saveAs();
    void saveCurrent();
    void scrollHtml(int);
    void swapWidget(int, int);
    void updateTab(bool);

};

#endif // MARKDOWN_EDITOR_H
