#include "markdown_textedit.hpp"

#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include <QFileDevice>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextBlock>
#include <QList>

const int PERMISSION_MASK = QFileDevice::ReadUser | QFileDevice::WriteUser;


MarkdownTextEdit::MarkdownTextEdit(const QString& file, QWidget* parent) : QPlainTextEdit(parent) {

    // MEMBERS DECLARATION
    mFile = new QFile(file, this);

    if(file != nullptr) {
        // User has the right to read the file
        if((mFile->permissions() &  PERMISSION_MASK) == PERMISSION_MASK) {
            // Open and read file pass into argument then hash the text
            if(mFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString text(mFile->readAll());
                this->setPlainText(text);
            }
            mFile->close();
        }
        else {
            QMessageBox::warning(this,
                                 "Permission Exception",
                                 "You don't have permission to read this file!");
            mFile->setFileName(nullptr);
        }
    }
    this->hashText();
}

// PUBLIC METHODS

bool MarkdownTextEdit::isSave() {
    /* Return save state by comparing last saved hash to current hash. */
    return mHashedText == qHash(this->toPlainText());
}

QFile* MarkdownTextEdit::file() {

    return mFile;
}


QString MarkdownTextEdit::parseCurrentBlock() const {

     int begin, end;

     QTextBlock currentBlock = this->textCursor().block();
     QTextBlock firstBlock   = this->document()->firstBlock();
     QTextBlock lastBlock    = this->document()->lastBlock();

     while(currentBlock.text().count() != 0 && currentBlock != firstBlock)
         currentBlock = currentBlock.previous();
     begin = currentBlock.position();

     currentBlock = this->textCursor().block();
     while(currentBlock.text().count() != 0 && currentBlock != lastBlock)
        currentBlock = currentBlock.next();
     end = currentBlock.position();

     QList<QString> listText;

     while(begin != end) {
         listText.append(this->document()->findBlockByNumber(begin++).text());
     }
     listText.append(this->document()->findBlockByNumber(end).text());

     return listText.join("\n");
 }

void MarkdownTextEdit::save() {
    /* Save text contain in the editor to current file path.
     * Calculate new hashed text value
     * Emit a save_signal with the success value.
     * If however, the file path is NULL, then the method shall
     * call the saveAs method to open a file dialog. */

    bool success;

    if(mFile->fileName().isNull()) {

        this->saveAs();
        return;
    }

    // Open stream to file path and write text into it
    if(mFile->open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream stream(mFile);
        stream << this->toPlainText();
        success = true;
    }
    else {

        success = false;
    }

    mFile->close();

    // Calculate new hash value
    this->hashText();

    // Emit save signal with success value
    emit signal_save(success);
}

void MarkdownTextEdit::saveAs() {
    /* File dialog for the user to choose new file name for mFile.
     * Return whether operation is success or not. */

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save As"),
                                                    QDir::homePath());
    if(!fileName.isEmpty()) {

        mFile->setFileName(fileName);

        if((mFile->permissions() & PERMISSION_MASK) == PERMISSION_MASK) {
            this->save();
        }
        else if(!mFile->exists()) {
            this->save();
        }
        else {

            QMessageBox::warning(this,
                                 "Permission Exception",
                                 "You don't have permission to read this file!");
            mFile->setFileName(nullptr);
        }
    }
}

// PRIVATE METHODS

void MarkdownTextEdit::hashText() {
    /* Calculate hashed text value from current text in editor */

    mHashedText = qHash(this->toPlainText());
}
