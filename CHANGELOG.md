# Changelog

### Version 1.1

- The renderer is now base on QWebEngine technologie instead of using the Qt's *scribe* framework (QTextBrowser). Because of that, the text editors that are stack can't be connect directly like it uses to. Instead, a QObject named **Document** is created and is linked to a QWebChannel. The **Document** always keep a copy of the current document been edited and send its content through the channel to the web engine.

- Because of the way the **QWebEngine** is build, I decided to use the **marked** library (*JavaScript*) instead of the **Discount** library (*C*) for markdown parsing.


- 