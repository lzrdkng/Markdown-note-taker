#include "markdown_tabbar.hpp"

#include <Qt>

MarkdownTabBar::MarkdownTabBar(QWidget* parent) : QTabBar(parent) {

    QPushButton* mAddTab = new QPushButton("+", this);

    mAddTab->setFixedSize(20, 20);
    this->placeAddTabButton();
}

QSize MarkdownTabBar::sizeHint() const {

    QSize sizeHint = QTabBar::sizeHint();
    int w  = sizeHint.width();
    int h = sizeHint.height();
    return QSize(w+25, h);

}


void MarkdownTabBar::resizeEvent(QResizeEvent* event) {
    QTabBar::resizeEvent(event);
    this->placeAddTabButton();
}

void MarkdownTabBar::tabLayoutChange() {
    QTabBar::tabLayoutChange();
    this->placeAddTabButton();
}

void MarkdownTabBar::placeAddTabButton() {

    int size = 0;
    for(int i=0;i<this->count();++i)
        size += this->tabRect(i).width();

    int h = this->geometry().top();
    int w = this->width();

    if(size>w)
        mAddTab->move(w-54, h);
    else
        mAddTab->move(size, h);
}
