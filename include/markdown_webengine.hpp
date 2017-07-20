#ifndef DEF_MARKDOWN_WEBENGINE_HPP
#define DEF_MARKDOWN_WEBENGINE_HPP

#include <QWidget>
#include <QWebEngineView>

class MarkdownWebEngine : public QWebEngineView {


public:

    MarkdownWebEngine(QWidget* parent = nullptr);
};


#endif
