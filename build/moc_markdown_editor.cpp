/****************************************************************************
** Meta object code from reading C++ file 'markdown_editor.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/markdown_editor.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'markdown_editor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MarkdownEditor_t {
    QByteArrayData data[18];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MarkdownEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MarkdownEditor_t qt_meta_stringdata_MarkdownEditor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MarkdownEditor"
QT_MOC_LITERAL(1, 15, 6), // "addTab"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 9), // "addWidget"
QT_MOC_LITERAL(4, 33, 17), // "MarkdownTextEdit*"
QT_MOC_LITERAL(5, 51, 9), // "newWidget"
QT_MOC_LITERAL(6, 61, 8), // "closeTab"
QT_MOC_LITERAL(7, 70, 5), // "index"
QT_MOC_LITERAL(8, 76, 8), // "openFile"
QT_MOC_LITERAL(9, 85, 14), // "markdownToHtml"
QT_MOC_LITERAL(10, 100, 11), // "refreshHtml"
QT_MOC_LITERAL(11, 112, 15), // "refreshTabColor"
QT_MOC_LITERAL(12, 128, 7), // "saveAll"
QT_MOC_LITERAL(13, 136, 6), // "saveAs"
QT_MOC_LITERAL(14, 143, 11), // "saveCurrent"
QT_MOC_LITERAL(15, 155, 10), // "scrollHtml"
QT_MOC_LITERAL(16, 166, 10), // "swapWidget"
QT_MOC_LITERAL(17, 177, 9) // "updateTab"

    },
    "MarkdownEditor\0addTab\0\0addWidget\0"
    "MarkdownTextEdit*\0newWidget\0closeTab\0"
    "index\0openFile\0markdownToHtml\0refreshHtml\0"
    "refreshTabColor\0saveAll\0saveAs\0"
    "saveCurrent\0scrollHtml\0swapWidget\0"
    "updateTab"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MarkdownEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    1,   95,    2, 0x0a /* Public */,
       3,    0,   98,    2, 0x2a /* Public | MethodCloned */,
       6,    1,   99,    2, 0x0a /* Public */,
       6,    0,  102,    2, 0x2a /* Public | MethodCloned */,
       8,    0,  103,    2, 0x0a /* Public */,
       9,    0,  104,    2, 0x0a /* Public */,
      10,    1,  105,    2, 0x0a /* Public */,
      10,    0,  108,    2, 0x2a /* Public | MethodCloned */,
      11,    0,  109,    2, 0x0a /* Public */,
      12,    0,  110,    2, 0x0a /* Public */,
      13,    0,  111,    2, 0x0a /* Public */,
      14,    0,  112,    2, 0x0a /* Public */,
      15,    1,  113,    2, 0x0a /* Public */,
      16,    2,  116,    2, 0x0a /* Public */,
      17,    1,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void MarkdownEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkdownEditor *_t = static_cast<MarkdownEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addTab(); break;
        case 1: _t->addWidget((*reinterpret_cast< MarkdownTextEdit*(*)>(_a[1]))); break;
        case 2: _t->addWidget(); break;
        case 3: _t->closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->closeTab(); break;
        case 5: _t->openFile(); break;
        case 6: _t->markdownToHtml(); break;
        case 7: _t->refreshHtml((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->refreshHtml(); break;
        case 9: _t->refreshTabColor(); break;
        case 10: _t->saveAll(); break;
        case 11: _t->saveAs(); break;
        case 12: _t->saveCurrent(); break;
        case 13: _t->scrollHtml((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->swapWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->updateTab((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MarkdownTextEdit* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MarkdownEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MarkdownEditor.data,
      qt_meta_data_MarkdownEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MarkdownEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MarkdownEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MarkdownEditor.stringdata0))
        return static_cast<void*>(const_cast< MarkdownEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MarkdownEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
