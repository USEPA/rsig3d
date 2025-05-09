/****************************************************************************
** Meta object code from reading C++ file 'urllineedit.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../urllineedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'urllineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExLineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_ExLineEdit[] = {
    "ExLineEdit\0"
};

void ExLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ExLineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ExLineEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ExLineEdit,
      qt_meta_data_ExLineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExLineEdit))
        return static_cast<void*>(const_cast< ExLineEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int ExLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_UrlLineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x08,
      41,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UrlLineEdit[] = {
    "UrlLineEdit\0\0url\0webViewUrlChanged(QUrl)\0"
    "webViewIconChanged()\0"
};

void UrlLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UrlLineEdit *_t = static_cast<UrlLineEdit *>(_o);
        switch (_id) {
        case 0: _t->webViewUrlChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: _t->webViewIconChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UrlLineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UrlLineEdit::staticMetaObject = {
    { &ExLineEdit::staticMetaObject, qt_meta_stringdata_UrlLineEdit,
      qt_meta_data_UrlLineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UrlLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UrlLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UrlLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UrlLineEdit))
        return static_cast<void*>(const_cast< UrlLineEdit*>(this));
    return ExLineEdit::qt_metacast(_clname);
}

int UrlLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ExLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
