#include "markdown_editor.hpp"
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

MarkdownEditor::MarkdownEditor() : QMainWindow() {

    // MEMBERS DECLARATION
    mTextBrowser   = new QTextBrowser(this);
    mSplitter      = new QSplitter(this);
    mStackedWidget = new QStackedWidget(this);
    mTabBar        = new QTabBar(this);
    mHighlighter   = new MarkdownHighlighter(this);

    // MEMBERS CUSTOMIZATION

    // Textbrowser
    mTextBrowser->document()->setDefaultStyleSheet(Markdown::DefaultTextBrowserStyle);
    mTextBrowser->document()->setDocumentMargin(20);

    // Tab Bar
    mTabBar->setMinimumWidth(128);
    mTabBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    mTabBar->setMovable(true);
    mTabBar->setTabsClosable(true);
    mTabBar->setDrawBase(true);
    mTabBar->setExpanding(false);
    mTabBar->setStyleSheet("QTabBar::tab {min-width:128px; max-width:512px;}");

    if(mTabBar->count() == 0)
        mTabBar->hide();

    // Splitter (central widget)
    mSplitter->addWidget(mStackedWidget);
    mSplitter->addWidget(mTextBrowser);
    mSplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));

    // Dock top
    QWidget*     dockWidget = new QWidget();
    QPushButton* addButton  = new QPushButton("+");
    QHBoxLayout* dockLayout = new QHBoxLayout();

    addButton->setMaximumWidth(30);

    dockLayout->addWidget(mTabBar, 0, Qt::AlignBottom);
    dockLayout->addWidget(addButton, 0, Qt::AlignLeft|Qt::AlignBottom);
    dockWidget->setLayout(dockLayout);

    QDockWidget* dock = new QDockWidget(this);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setWidget(dockWidget);
    dock->setFixedHeight(88);

    // ACTIONS

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

    // SLOTS & SIGNALS
    QObject::connect(addButton,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(addTab()));
    QObject::connect(mTabBar,
                     SIGNAL(tabCloseRequested(int)),
                     this,
                     SLOT(closeTab(int)));
    QObject::connect(mTabBar,
                     SIGNAL(currentChanged(int)),
                     this,
                     SLOT(refreshHtml(int)));
    QObject::connect(mTabBar,
                     SIGNAL(tabMoved(int,int)),
                     this,
                     SLOT(swapWidget(int,int)));


    // MENU BAR
    QMenu* fileMenu = new QMenu("&File", this);
    fileMenu->addActions(actions);
    this->menuBar()->addMenu(fileMenu);

    // ROOT CONFIGURATIONS

    // Layout
    this->addDockWidget(Qt::TopDockWidgetArea, dock, Qt::Horizontal);
    this->setCentralWidget(mSplitter);

    // Geometry and misc
    this->resize(1200, 400);
    this->setWindowTitle("~Lizard~");
}

// PUBLIC METHODS

// PRIVATE METHODS

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
    }

}

// PRIVATE METHODS

void MarkdownEditor::removeWidget(int index) {
    /* Remove widget at position index in stack */

    MarkdownTextEdit* widget = (MarkdownTextEdit*)mStackedWidget->widget(index);

    if(widget == nullptr)
        /* This error is not suppose to occur except if someone pass an invalid
         * index in parameter. If however this error occurs, please make sure
         * that the index pass in argument is a valid index in the stacked
         * widget. */
        QMessageBox::critical(this,
                              "Lizard - NULL POINTER EXCEPTION",
                              "A nullptr has been detected in private method "
                              "'MarkdownEditor::removeWidget'. Please, report this incident!");
    else {
        mStackedWidget->removeWidget(widget);
        delete widget;
    }
}

// PUBLIC SLOTS

void MarkdownEditor::addTab() {
    /* Add an empty tab in mTabBar.
     * Add a new widget to the stack. */

    int index = mTabBar->addTab("new file");

    this->addWidget();

    mTabBar->setCurrentIndex(index); // Triggered currentChanged signal
}

void MarkdownEditor::addWidget(MarkdownTextEdit* newWidget) {
    /* Add a widget at end of the stack */

    if(newWidget == nullptr)
        newWidget = new MarkdownTextEdit(NULL);

    mStackedWidget->addWidget(newWidget); // mStackedWidget take ownership of newWidget

    // SIGNALS AND SLOTS CONNECTION
    QObject::connect(newWidget->verticalScrollBar(),
                     SIGNAL(valueChanged(int)),
                     this,
                     SLOT(scrollHtml(int)));
    QObject::connect(newWidget,
                     SIGNAL(textChanged()),
                     this,
                     SLOT(markdownToHtml()));
    QObject::connect(newWidget,
                     SIGNAL(signal_save(bool)),
                     this,
                     SLOT(updateTab(bool)));

    mHighlighter->setDocument(newWidget->document());
}

void MarkdownEditor::closeTab(int index) {
    /* Close tab at index */

    if(index == -1) // Action triggered
        index = mTabBar->currentIndex(); // Will close the current tab

    MarkdownTextEdit* tab = (MarkdownTextEdit*)mStackedWidget->widget(index);

    if(tab != nullptr) {
        // Check if the current tab save state
        if(!tab->isSave()) {
            QMessageBox* reply        = new QMessageBox(this);
            reply->setDefaultButton(QMessageBox::Save);
            reply->setEscapeButton(QMessageBox::Cancel);
            reply->setIcon(QMessageBox::Question);
            reply->setInformativeText(mTabBar->tabText(index));
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

        this->removeWidget(index);
        mTabBar->removeTab(index);
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

        MarkdownTextEdit* newWidget = new MarkdownTextEdit(fileName);

        int index = mTabBar->addTab(fileName);

        this->addWidget(newWidget);

        mTabBar->setCurrentIndex(index);
    }
}

void MarkdownEditor::refreshHtml(int index) {
    /* Refresh the text in the TextBrowser depending
     * on the widget at position index in stack */

    // If no editor open, hide the tab bar
    if(index == -1) {
        mTabBar->hide();
    }
    else {
        mTabBar->show();

        mStackedWidget->setCurrentIndex(index);

        MarkdownTextEdit* current = (MarkdownTextEdit*)mStackedWidget->currentWidget();

        // Switch the document of the highlighter before assigning new text
        if(current == nullptr)
            mHighlighter->setDocument(nullptr);
        else
            mHighlighter->setDocument(current->document());
    }

    this->markdownToHtml();
}

void MarkdownEditor::refreshTabColor() {
    /* Refresh the text color of the current tab in the tab bar */

    int index = mTabBar->currentIndex();

    MarkdownTextEdit* currentTab = (MarkdownTextEdit*)mStackedWidget->currentWidget();

    if(currentTab != nullptr) {
        if(!currentTab->isSave())
            mTabBar->setTabTextColor(index, QColor::fromRgb(255, 0, 0)); // Red color if needs to be save
        else
            mTabBar->setTabTextColor(index, QColor::fromRgb(0, 0, 0));   // Otherwise, black color
    }
}

void MarkdownEditor::saveAll() {
    /* Save all files */

    MarkdownTextEdit* ptr;

    for(int i=0;i<mStackedWidget->count();++i) {
        ptr = (MarkdownTextEdit*)mStackedWidget->widget(i);
        ptr->save();
    }
}

void MarkdownEditor::saveAs() {
    /* Call saveAs method of the current widget in the stack */

    MarkdownTextEdit* current = (MarkdownTextEdit*)mStackedWidget->currentWidget();

    if(current != nullptr)
        current->saveAs();
}

void MarkdownEditor::saveCurrent() {
    /* Call save method of the current widget in stack */

    MarkdownTextEdit* current_tab = (MarkdownTextEdit*)mStackedWidget->currentWidget();

    if(current_tab != nullptr)
        current_tab->save();
}

void MarkdownEditor::scrollHtml(int markdownPos) {
    /* Synchronize the TextBrowser vertical scrollbar with the
     * current widget in the stack, by applying a ratio rule. */

    MarkdownTextEdit* currentWidget = (MarkdownTextEdit*)mStackedWidget->currentWidget();

    if(currentWidget != nullptr) {

        int markdownMax = currentWidget->verticalScrollBar()->maximum();

        if(markdownMax == 0)
            mTextBrowser->verticalScrollBar()->setValue(0);
        else {
            int htmlMax  = mTextBrowser->verticalScrollBar()->maximum();
            double ratio = htmlMax/(double)markdownMax;

            mTextBrowser->verticalScrollBar()->setValue(markdownPos * ratio);
        }
    }
}

void MarkdownEditor::swapWidget(int i, int j) {
    /* Swap widget at position i with widget at position j in stack */

    MarkdownTextEdit* a = (MarkdownTextEdit*)mStackedWidget->widget(i);
    MarkdownTextEdit* b = (MarkdownTextEdit*)mStackedWidget->widget(j);

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

void MarkdownEditor::markdownToHtml() {
    /* Translate the markdown expression in the current widget in stack
     * to html and put it in the text browser.
     * http://www.pell.portland.or.us/~orc/Code/discount/ */

    MarkdownTextEdit* current = (MarkdownTextEdit*)mStackedWidget->currentWidget();

    if(current == nullptr) {
        mTextBrowser->setText("");
    }
    else {
        /* To translate the mardown to html, I use the markdown discout lib.
         * Because it's a C lib, C data type are use.
         *
         * The following steps are made to translate the markdown to html.
         *
         * 1. Fetch the text
         * 2. Convert the text to C standard char pointer and keep its size
         * 3. Create a MMIOT pointer from the text and its size
         * 4. Open a temporary file
         * 5. Store the html into the temporary file from the MMIOT document
         * 6. Rewin the temporary file and recreate a QString from it */

        std::string text   = current->toPlainText().toStdString(); // Fetch text in the text editor
        const char* text_c = text.c_str(); // Store it in a C char pointer
        int size = text.size(); // Store its size as well
        MMIOT* doc = mkd_string(text_c, // Create a MMIOT document pointer from the text and its size
                                size,
                                MKD_NOSTYLE    |
                                MKD_NOLINKS    |
                                MKD_NOIMAGE    |
                                MKD_NOHEADER   |
                                MKD_NODIVQUOTE |
                                MKD_FENCEDCODE);

        FILE* out = tmpfile();

        if(out != NULL) {
            QString str;
            char buffer[256];

            // Will free MMIOT document
            markdown(doc,
                     out,
                     MKD_NOSTYLE    |
                     MKD_NOLINKS    |
                     MKD_NOIMAGE    |
                     MKD_NOHEADER   |
                     MKD_NODIVQUOTE |
                     MKD_FENCEDCODE);
            rewind(out);

            // PERHAP A BETTER TO DO THIS
            while(!feof(out)) {
                if(fgets(buffer, 256, out) == NULL)
                    break;
                str.append(QString::fromLocal8Bit(buffer));
            }

            mTextBrowser->setText(str);
        }
        else {
            /* If the system failed to create a temporary file, we need to
             * free the MMIOT document from memory. This function is however
             * called if the file is open */
            mkd_cleanup(doc);
        }
        fclose(out);

        this->scrollHtml(current->verticalScrollBar()->value());
        this->refreshTabColor();
    }
}

void MarkdownEditor::updateTab(bool saveSuccess) {
    /* Update the tab text corresponding to the file name opened */

    if(!saveSuccess)
        QMessageBox::warning(this,
                             tr("Lizard - Failed to Save."),
                             tr("Lizard failed to save your file!"));
    else {

        MarkdownTextEdit* currentTab = (MarkdownTextEdit*)mStackedWidget->currentWidget();

        if(currentTab != nullptr)
            mTabBar->setTabText(mTabBar->currentIndex(), currentTab->file()->fileName());
    }

    this->refreshTabColor();
}
