/****************************************************************************
** Meta object code from reading C++ file 'embeddeddialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../embeddeddialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'embeddeddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EmbeddedDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   16,   15,   15, 0x08,
      58,   50,   15,   15, 0x08,
      83,   78,   15,   15, 0x08,
     112,  102,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EmbeddedDialog[] = {
    "EmbeddedDialog\0\0index\0layoutDirectionChanged(int)\0"
    "spacing\0spacingChanged(int)\0font\0"
    "fontChanged(QFont)\0styleName\0"
    "styleChanged(QString)\0"
};

void EmbeddedDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EmbeddedDialog *_t = static_cast<EmbeddedDialog *>(_o);
        switch (_id) {
        case 0: _t->layoutDirectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->spacingChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->fontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 3: _t->styleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EmbeddedDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EmbeddedDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EmbeddedDialog,
      qt_meta_data_EmbeddedDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EmbeddedDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EmbeddedDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EmbeddedDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EmbeddedDialog))
        return static_cast<void*>(const_cast< EmbeddedDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int EmbeddedDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
