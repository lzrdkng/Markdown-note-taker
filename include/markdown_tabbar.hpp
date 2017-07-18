#ifndef DEF_MARKDOWN_TABBAR_HPP
#define DEF_MARKDOWN_TABBAR_HPP

#include <QWidget>
#include <QResizeEvent>
#include <QTabBar>
#include <QPushButton>
#include <QSize>

class MarkdownTabBar : public QTabBar {

    Q_OBJECT

public:

    MarkdownTabBar(QWidget* parent = nullptr);

    QSize sizeHint() const;

protected:

    void resizeEvent(QResizeEvent* event);
    void tabLayoutChange();

private:

    QPushButton* mAddTab;

    void placeAddTabButton();


public slots:


};

#endif
