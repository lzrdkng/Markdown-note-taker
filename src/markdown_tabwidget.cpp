#include "markdown_tabwidget.hpp"

#include <QMessageBox>
#include <QSizePolicy>

MarkdownTabWidget::MarkdownTabWidget(QWidget* parent) : QWidget(parent) {

    mTabBar        = new QTabBar(this);
    mStackedWidget = new QStackedWidget(this);

    mTabBar->setAutoHide(false);
    mTabBar->setMinimumWidth(128);
    mTabBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    mTabBar->setMovable(true);
    mTabBar->setTabsClosable(true);
    mTabBar->setDrawBase(true);
    mTabBar->setExpanding(false);
    mTabBar->setElideMode(Qt::ElideLeft);

    QObject::connect(mTabBar,
                     SIGNAL(currentChanged(int)),
                     mStackedWidget,
                     SLOT(setCurrentIndex(int)));

    QObject::connect(mTabBar,
                     SIGNAL(tabMoved(int,int)),
                     this,
                     SLOT(swapWidget(int,int)));
}


void MarkdownTabWidget::addWidget(QWidget* widget, const QString& label) {

    mStackedWidget->addWidget(widget);
    mTabBar->addTab(label);
}

void MarkdownTabWidget::clear() {

    for(int i=0;i<mTabBar->count();++i)
        mTabBar->tabCloseRequested(i);
}

void MarkdownTabWidget::removeTab(int index) {
    QWidget* w = mStackedWidget->widget(index);
    mStackedWidget->removeWidget(w);
    delete w;
    mTabBar->removeTab(index);
}

QStackedWidget* MarkdownTabWidget::stack() {
    return mStackedWidget;
}

QTabBar* MarkdownTabWidget::tabBar() {
    return mTabBar;
}

void MarkdownTabWidget::swapWidget(int i, int j) {

    QWidget* a = mStackedWidget->widget(i);
    QWidget* b = mStackedWidget->widget(j);

    if(a == nullptr || b == nullptr)
        /* The swapWidget slot is connected to the tabMoved signal of TabBar.
         * This error will appear only if the stack is not correctly
         * synchronize with the TabBar. i.e that all call to
         * [addTab, removeTab, insertTab] must somehow be related
         * to the corresponding method of the stack. */
        QMessageBox::critical(this,
                              "Lizard - NULL POINTER EXCEPTION",
                              "A nullptr has been detected in slot "
                              "'MarkdownEditor::swapWidget'. Please, report this incident!");

    mStackedWidget->removeWidget(a);
    mStackedWidget->insertWidget(j, a);

    mStackedWidget->removeWidget(b);
    mStackedWidget->insertWidget(i, b);
}
