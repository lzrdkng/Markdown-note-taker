#ifndef DEF_MARKDOWN_TABWIDGET_HPP
#define DEF_MARKDOWN_TABWIDGET_HPP

#include <QObject>
#include <QTabBar>
#include <QStackedWidget>
#include <QString>

class MarkdownTabWidget : public QWidget {

    Q_OBJECT

public:
    MarkdownTabWidget(QWidget* parent = nullptr);

    void addWidget(QWidget* widget, const QString& label);

    void clear();

    void removeTab(int index);

    QStackedWidget* stack();

    QTabBar* tabBar();

    QWidget* widget(int index);

private slots:


    void swapWidget(int i, int j);

private:

    QTabBar*        mTabBar;
    QStackedWidget* mStackedWidget;

};

#endif
