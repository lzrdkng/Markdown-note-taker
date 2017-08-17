#include "markdown_highlighter.hpp"

#include <QColor>
#include <Qt>

MarkdownHighlighter::MarkdownHighlighter(QObject* parent) : QSyntaxHighlighter(parent){

    HighlightingRule rule;

    // SINGLE LINE EXPRESSIONS


    // Quota markdown >.+

    QFont quoteFont;

    quoteFont.setItalic(true);
    quoteFont.setPixelSize(18);
    quoteFormat.setFont(quoteFont);

    QColor quoteBackground;

    quoteBackground.setRgb(128, 128, 128, 64);

    quoteFormat.setForeground(QColor::fromRgb(0, 170, 221));
    quoteFormat.setBackground(quoteBackground);

    rule.pattern = QRegularExpression("^>.+$", QRegularExpression::DotMatchesEverythingOption); // OK
    rule.format  = quoteFormat;
    highlightingRules.append(rule);


    // Header markdown #{1,6}\\s+.+

    QFont headerFont;

    headerFont.setPixelSize(32);
    headerFont.setWeight(QFont::ExtraBold);
    headerFormat.setFont(headerFont);

    headerFormat.setForeground(QColor::fromRgb(192, 0, 255));

    rule.pattern = QRegularExpression("^#{1,6}\\s+.+$", QRegularExpression::DotMatchesEverythingOption); // OK
    rule.format  = headerFormat;
    highlightingRules.append(rule);


    // Break markdown ---

    QFont breakFont;

    breakFont.setBold(true);
    breakFont.setPixelSize(24);
    breakFormat.setFont(breakFont);

    breakFormat.setForeground(QColor::fromRgb(0, 80, 0));

    rule.pattern = QRegularExpression("^---|\\*\\*\\*|___$"); // OK
    rule.format  = breakFormat;
    highlightingRules.append(rule);


    // Italic markdown *\.+?*

    QFont italicFont;

    italicFont.setItalic(true);
    italicFont.setPixelSize(20);
    italicFormat.setFont(italicFont);

    italicFormat.setForeground(QColor::fromRgb(192, 135, 66));

    rule.pattern = QRegularExpression("(?<i>[_*])(.+?)\\g<i>"); // OK
    rule.format  = italicFormat;
    highlightingRules.append(rule);


    // Bold markdown **\.+?**

    QFont boldFont;

    boldFont.setBold(true);
    boldFont.setPixelSize(20);
    boldFormat.setFont(boldFont);

    boldFormat.setForeground(QColor::fromRgb(0, 64, 128));

    rule.pattern = QRegularExpression("(?<b>[_*]){2}(.+?)\\g<b>{2}"); // OK
    rule.format  = boldFormat;
    highlightingRules.append(rule);


    // List markdown

    QFont listFont;

    listFont.setBold(true);
    listFont.setPixelSize(20);
    listFormat.setFont(listFont);

    listFormat.setForeground(QColor::fromRgb(192, 63, 42));

    rule.pattern = QRegularExpression("^\\s*([-*]|(\\d\\.))\\s"); // OK
    rule.format  = listFormat;
    highlightingRules.append(rule);


    // Strike markdown ~~\S+~~

    QFont strikeFont;

    strikeFont.setBold(true);
    strikeFont.setPixelSize(20);
    strikeFormat.setFont(strikeFont);

    strikeFormat.setForeground(Qt::darkBlue);

    rule.pattern = QRegularExpression("^~~\\S+~~$");
    rule.format  = strikeFormat;
    highlightingRules.append(rule);


    // Single line code \t\w+

    QFont codeFont;

    codeFont.setItalic(true);
    codeFont.setPixelSize(20);
    codeLineFormat.setFont(codeFont);

    QColor codeBackground;
    codeBackground.setRgb(64, 64, 64, 32);

    codeLineFormat.setForeground(Qt::darkGray);
    codeLineFormat.setBackground(codeBackground);

    rule.pattern = QRegularExpression("\\t{1,}.+$");
    rule.format  = codeLineFormat;
    highlightingRules.append(rule);


    // MULTI LINES EXPRESSIONS

    codeFormat.setFont(codeFont);

    codeFormat.setForeground(Qt::darkGray);
    codeFormat.setBackground(codeBackground);

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
