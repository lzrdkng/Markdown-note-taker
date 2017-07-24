#ifndef MARKDOWN_TEXTEDIT_HPP
#define MARKDOWN_TEXTEDIT_HPP

#include <QFile>
#include <QPlainTextEdit>

class MarkdownTextEdit : public QPlainTextEdit {

    Q_OBJECT

public:

    MarkdownTextEdit(const QString& file, QWidget* parent = nullptr);

    // Return save state of the editor
    bool isSave();

    // Return pointer to mFile
    QFile* file();

    // Save text to file
    void save();

    // Save text to file as
    void saveAs();

private:

    int             mCurrentblock;
    QFile*          mFile;
    unsigned int    mHashedText;

    // Hash current text
    void hashText();

signals:

    void signal_save(bool success);

};

#endif
