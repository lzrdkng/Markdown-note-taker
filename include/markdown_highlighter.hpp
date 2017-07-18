#ifndef DEF_MARKDOWN_HIGHLIGHTER_HPP
#define DEF_MARKDOWN_HIGHLIGHTER_HPP

#include <QObject>
#include <QSyntaxHighlighter>
#include <QString>
#include <QVector>
#include <QRegularExpression>
#include <QTextCharFormat>

typedef QTextCharFormat Format;

class MarkdownHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

public:

    MarkdownHighlighter(QObject* parent = nullptr);

protected:

    void highlightBlock(const QString& text) override;

private:

    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression codeStartExpression;
    QRegularExpression codeEndExpression;

    Format boldFormat;
    Format breakFormat;
    Format codeFormat;
    Format emphaseFormat;
    Format headerFormat;
    Format listFormat;
    Format quoteFormat;
    Format centerFormat;
    Format strikeFormat;

};



#endif
