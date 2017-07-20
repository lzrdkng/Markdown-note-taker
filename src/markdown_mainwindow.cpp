#include "markdown_mainwindow.hpp"
#include "constants.hpp"

extern "C" {
#include <stdio.h>
#include <mkdio.h>
}

#include <QAction>
#include <QColor>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScrollBar>
#include <QSizePolicy>
#include <Qt>
#include <QWidget>

#include <iostream>
#include <QUrl>
#include <QWebChannel>


MarkdownEditor::MarkdownEditor(QWidget* parent) :
    QMainWindow(parent) {

    this->setupUI();
    this->setupActions();
    this->setupConnections();
}


// PRIVATE METHODS

void MarkdownEditor::setupActions() {

    QAction* actionQuit = new QAction("Quit", this);
    actionQuit->setShortcut(QKeySequence::Quit);
    QObject::connect(actionQuit,
                     SIGNAL(triggered()),
                     this,
                     SLOT(close()));

    QAction* actionSaveCurrent = new QAction("Save", this);
    actionSaveCurrent->setShortcut(QKeySequence::Save);
    QObject::connect(actionSaveCurrent,
                     SIGNAL(triggered()),
                     this,
                     SLOT(saveCurrent()));


    QAction* actionCloseTab = new QAction("Close Tab", this);
    actionCloseTab->setShortcut(QKeySequence::Close);
    QObject::connect(actionCloseTab,
                     SIGNAL(triggered()),
                     this,
                     SLOT(closeTab()));

    QAction* actionNewFile = new QAction("New File", this);
    actionNewFile->setShortcut(QKeySequence::New);
    QObject::connect(actionNewFile,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(addTab()));

    QAction* actionOpenFile = new QAction("Open File", this);
    actionOpenFile->setShortcut(QKeySequence::Open);
    QObject::connect(actionOpenFile,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(openFile()));

    QAction* actionSaveAll = new QAction("Save All", this);
    QObject::connect(actionSaveAll,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(saveAll()));

    QAction* actionSaveAs = new QAction("Save As", this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    QObject::connect(actionSaveAs,
                     SIGNAL(triggered(bool)),
                     this,
                     SLOT(saveAs()));

    // Seperator action
    QAction* actionSeparator1 = new QAction(this);
    QAction* actionSeparator2 = new QAction(this);
    actionSeparator1->setSeparator(true);
    actionSeparator2->setSeparator(true);


    QList<QAction*> actions = {
        actionNewFile,
        actionOpenFile,
        actionSeparator1,
        actionSaveCurrent,
        actionSaveAs,
        actionSaveAll,
        actionSeparator2,
        actionCloseTab,
        actionQuit
    };
    this->addActions(actions);

    QMenu* fileMenu = new QMenu("&File", this);
    fileMenu->addActions(actions);
    this->menuBar()->addMenu(fileMenu);

}

void MarkdownEditor::setupConnections() {

    QObject::connect(mTabWidget->tabBar(),
                     SIGNAL(tabCloseRequested(int)),
                     this,
                     SLOT(closeTab(int)));
    QObject::connect(mTabWidget->tabBar(),
                     SIGNAL(currentChanged(int)),
                     this,
                     SLOT(swapDocument(int)));

    QWebChannel* channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), mDocument);
    mWebEngine->page()->setWebChannel(channel);
}


void MarkdownEditor::setupUI() {

    mSplitter      = new QSplitter(this);
    mTabWidget     = new MarkdownTabWidget(this);
    mHighlighter   = new MarkdownHighlighter(this);
    mWebEngine     = new MarkdownWebEngine(this);
    mDocument      = new MarkdownDocument((QObject*)this);

    mWebEngine->setUrl(QUrl("qrc:///html/index.html"));

    mSplitter->addWidget(mTabWidget->stack());
    mSplitter->addWidget(mWebEngine);
    mSplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    QWidget*     dockWidget = new QWidget();
    QPushButton* addButton  = new QPushButton("+");
    QObject::connect(addButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(addTab()));
    QHBoxLayout* dockLayout = new QHBoxLayout();

    addButton->setMaximumWidth(30);

    dockLayout->addWidget(mTabWidget->tabBar(), 0, Qt::AlignBottom);
    dockLayout->addWidget(addButton, 0, Qt::AlignLeft|Qt::AlignBottom);
    dockWidget->setLayout(dockLayout);

    QDockWidget* dock = new QDockWidget(this);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setWidget(dockWidget);
    dock->setFixedHeight(88);

    this->addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Horizontal);
    this->setCentralWidget(mSplitter);

    this->resize(1200, 400);
    this->setWindowTitle("~Lizard~");
}

// PROTECTED METHODS

void MarkdownEditor::closeEvent(QCloseEvent *event) {
    /* Catch close event of main window.
     * Check all tab to see if any editor need to be save.
     * Open a message box asking the user if he wants to
     * Save all | Cancel | Discard.
     *
     * If Save all selected, call saveAll method and accept the event.
     * If Cancel selected, ignore the event.
     * If Discard selected, accept the event.
     */
/*
    QList<QString> unsavedFiles;
    MarkdownTextEdit* ptr;

    // Fetch unsaved files name
    for(int i=0;i<mStackedWidget->count();i++) {
        ptr = (MarkdownTextEdit*)mStackedWidget->widget(i);
        if(!ptr->isSave())
            unsavedFiles.append(mTabBar->tabText(i));
    }

    if(unsavedFiles.count()) {
        QMessageBox* reply = new QMessageBox(this);
        reply->setDefaultButton(QMessageBox::SaveAll);
        reply->setEscapeButton(QMessageBox::Cancel);
        reply->setIcon(QMessageBox::Question);
        reply->setInformativeText(unsavedFiles.join("\n")); // Display all files that need to be save
        reply->setStandardButtons(QMessageBox::Discard |
                                  QMessageBox::SaveAll |
                                  QMessageBox::Cancel);
        reply->setText(tr("The following files have unsaved changes: "));
        reply->setWindowTitle("Save Changes");

        int buttonCode = reply->exec();

        delete reply;

        switch(buttonCode) {

        case QMessageBox::SaveAll:
            this->saveAll();
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        case QMessageBox::Discard:
            event->accept();
            break;
        default:
            event->accept();
        }
    }*/
    event->accept();

}

// PUBLIC SLOTS

void MarkdownEditor::addTab() {
    this->addTab(nullptr);
}

void MarkdownEditor::addTab(MarkdownTextEdit* newEditor) {
    /* Add an empty tab in mTabBar.
     * Add a new widget to the stack. */

    if(newEditor == nullptr)
        newEditor = new MarkdownTextEdit(nullptr);


    QObject::connect(newEditor,
                     SIGNAL(signal_save(bool)),
                     this,
                     SLOT(updateTab(bool)));
    QObject::connect(newEditor,
                     SIGNAL(textChanged()),
                     this,
                     SLOT(refreshDocument()));

    mTabWidget->addWidget(newEditor, "new file");
}

void MarkdownEditor::closeTab(int index) {
    /* Close tab at index */

    if(index == -1) // Action triggered
        index = mTabWidget->tabBar()->currentIndex(); // Will close the current tab

    MarkdownTextEdit* tab = (MarkdownTextEdit*)mTabWidget->stack()->widget(index);

    if(tab != nullptr) {
        // Check if the current tab save state
        if(!tab->isSave()) {
            QMessageBox* reply        = new QMessageBox(this);
            reply->setDefaultButton(QMessageBox::Save);
            reply->setEscapeButton(QMessageBox::Cancel);
            reply->setIcon(QMessageBox::Question);
            reply->setInformativeText(mTabWidget->tabBar()->tabText(index));
            reply->setStandardButtons(QMessageBox::Discard |
                                      QMessageBox::Cancel  |
                                      QMessageBox::Save);
            reply->setText("The following file has unsaved changes: ");
            reply->setWindowTitle(tr("Lizard - Save Changes"));

            int buttonCode = reply->exec();

            delete reply; // Make sure to delete the dialog

            switch(buttonCode) {

            case QMessageBox::Save:
                tab->save();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return; // Cancel all actions
            default:
                break;
            }
        }
        mTabWidget->removeTab(index);
    }
}

void MarkdownEditor::openFile() {
    /* Open a file from the user system.
     * Store the text of the file in a new widget and put it
     * in the stack. Add a new tab bar. */

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Lizard - Open File"),
                                                    QDir::homePath());
    if(!fileName.isEmpty()) {

        MarkdownTextEdit* newEditor = new MarkdownTextEdit(fileName);

        this->addTab(newEditor);
    }
}

void MarkdownEditor::refreshDocument(int index) {

    if(index == -1) // triggered by textChanged of current editor
        index = mTabWidget->tabBar()->currentIndex();

    MarkdownTextEdit* currentEditor = (MarkdownTextEdit*)mTabWidget->stack()->widget(index);

    if(currentEditor != nullptr) {
        mDocument->setText(currentEditor->toPlainText());
        this->refreshTabColor();
    }
    else
        mDocument->setText("");
}

void MarkdownEditor::refreshTabColor() {
    /* Refresh the text color of the current tab in the tab bar */

    int index = mTabWidget->tabBar()->currentIndex();

    MarkdownTextEdit* currentTab = (MarkdownTextEdit*)mTabWidget->stack()->currentWidget();

    if(currentTab != nullptr) {
        if(!currentTab->isSave())
            mTabWidget->tabBar()->setTabTextColor(index, QColor::fromRgb(255, 0, 0)); // Red color if needs to be save
        else
            mTabWidget->tabBar()->setTabTextColor(index, QColor::fromRgb(0, 0, 0));   // Otherwise, black color
    }
}

void MarkdownEditor::saveAll() {
    /* Save all files */

    MarkdownTextEdit* ptr;
    QStackedWidget* stack = mTabWidget->stack();

    for(int i=0;i<stack->count();++i) {
        ptr = (MarkdownTextEdit*)stack->widget(i);
        ptr->save();
    }
}

void MarkdownEditor::saveAs() {
    /* Call saveAs method of the current widget in the stack */

    MarkdownTextEdit* current = (MarkdownTextEdit*)mTabWidget->stack()->currentWidget();

    if(current != nullptr)
        current->saveAs();
}

void MarkdownEditor::saveCurrent() {
    /* Call save method of the current widget in stack */

    MarkdownTextEdit* current_tab = (MarkdownTextEdit*)mTabWidget->stack()->currentWidget();

    if(current_tab != nullptr)
        current_tab->save();
}

void MarkdownEditor::swapDocument(int index) {

    if(index == -1)
        mHighlighter->setDocument(nullptr);
    else {
        MarkdownTextEdit* currentEditor = (MarkdownTextEdit*)mTabWidget->stack()->widget(index);
        mHighlighter->setDocument(currentEditor->document());
    }

}

void MarkdownEditor::updateTab(bool saveSuccess) {
    /* Update the tab text corresponding to the file name opened */

    if(!saveSuccess)
        QMessageBox::warning(this,
                             tr("Lizard - Failed to Save."),
                             tr("Lizard failed to save your file!"));
    else {

        MarkdownTextEdit* currentTab = (MarkdownTextEdit*)mTabWidget->stack()->currentWidget();

        if(currentTab != nullptr)
            mTabWidget->tabBar()->setTabText(mTabWidget->tabBar()->currentIndex(), currentTab->file()->fileName());
    }

    this->refreshTabColor();
}
