/****************************************************************************
** Meta object code from reading C++ file 'richtexteditor_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shared/richtexteditor_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'richtexteditor_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdesigner_internal__RichTextEditorDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      51,   42,   41,   41, 0x08,
      72,   41,   41,   41, 0x08,
      90,   41,   41,   41, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdesigner_internal__RichTextEditorDialog[] = {
    "qdesigner_internal::RichTextEditorDialog\0"
    "\0newIndex\0tabIndexChanged(int)\0"
    "richTextChanged()\0sourceChanged()\0"
};

void qdesigner_internal::RichTextEditorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RichTextEditorDialog *_t = static_cast<RichTextEditorDialog *>(_o);
        switch (_id) {
        case 0: _t->tabIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->richTextChanged(); break;
        case 2: _t->sourceChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qdesigner_internal::RichTextEditorDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qdesigner_internal::RichTextEditorDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_qdesigner_internal__RichTextEditorDialog,
      qt_meta_data_qdesigner_internal__RichTextEditorDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qdesigner_internal::RichTextEditorDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qdesigner_internal::RichTextEditorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qdesigner_internal::RichTextEditorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdesigner_internal__RichTextEditorDialog))
        return static_cast<void*>(const_cast< RichTextEditorDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int qdesigner_internal::RichTextEditorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
