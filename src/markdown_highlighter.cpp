#include "markdown_highlighter.hpp"

#include <QColor>

MarkdownHighlighter::MarkdownHighlighter(QObject* parent) : QSyntaxHighlighter(parent){

    HighlightingRule rule;

    // SINGLE LINE EXPRESSIONS

    quoteFormat.setForeground(QColor::fromRgb(0, 170, 221));
    rule.pattern = QRegularExpression("^>.+$", QRegularExpression::DotMatchesEverythingOption); // OK
    rule.format  = quoteFormat;
    highlightingRules.append(rule);


    headerFormat.setForeground(QColor::fromRgb(192, 0, 255));
    rule.pattern = QRegularExpression("^#{1,6}.+$", QRegularExpression::DotMatchesEverythingOption); // OK
    rule.format  = headerFormat;
    highlightingRules.append(rule);

    breakFormat.setForeground(QColor::fromRgb(0, 80, 0));
    rule.pattern = QRegularExpression("^---|\\*\\*\\*|___$"); // OK
    rule.format  = breakFormat;
    highlightingRules.append(rule);

    boldFormat.setForeground(QColor::fromRgb(128, 64, 0));
    rule.pattern = QRegularExpression("(?<b>[_*]){1,2}(.+)\\g<b>{1,2}"); // OK
    rule.format  = boldFormat;
    highlightingRules.append(rule);

    listFormat.setForeground(QColor::fromRgb(192, 0, 0));
    rule.pattern = QRegularExpression("^\\s*([-*]|(\\d\\.))\\s"); // OK
    rule.format  = listFormat;
    highlightingRules.append(rule);

    centerFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("^(->).+(<-)$"); // OK
    rule.format  = centerFormat;
    highlightingRules.append(rule);

    strikeFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression("^~~~.+~~~$");
    rule.format  = strikeFormat;
    highlightingRules.append(rule);


    // MULTI LINES EXPRESSIONS

    codeFormat.setForeground(Qt::blue);

    codeStartExpression = QRegularExpression("^~~~\\w+$");
    codeEndExpression   = QRegularExpression("^~~~$"); // OK
}


void MarkdownHighlighter::highlightBlock(const QString &text)
{
    // SINGLE LINE

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }

    }

    setCurrentBlockState(0);

    // CODE FORMAT

    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = text.indexOf(codeStartExpression);

    while(startIndex >= 0) {

        QRegularExpressionMatch match = codeEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int codeLength = 0;
        if(endIndex == -1) {
            setCurrentBlockState(1);
            codeLength = text.length() - startIndex;
        }
        else {
            codeLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, codeLength, codeFormat);
        startIndex = text.indexOf(codeStartExpression, startIndex + codeLength);
    }

}
