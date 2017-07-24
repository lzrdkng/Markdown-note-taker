#include "markdown_document.hpp"

void MarkdownDocument::setText(const QString &text) {

    // Skip the signal if text hasn't changed
    if(text == mText)
        return;
    mText = text;
    emit textChanged(mText);
}
