#ifndef DEF_MARKDOWN_DOCUMENT_HPP
#define DEF_MARKDOWN_DOCUMENT_HPP

#include <QObject>
#include <QString>

class MarkdownDocument : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString text MEMBER mText NOTIFY textChanged FINAL)

public:

    explicit MarkdownDocument(QObject* parent= nullptr) : QObject(parent) {}

    void setText(const QString& text);

private:

    QString mText;

signals:
    void textChanged(const QString& text);

};

#endif
