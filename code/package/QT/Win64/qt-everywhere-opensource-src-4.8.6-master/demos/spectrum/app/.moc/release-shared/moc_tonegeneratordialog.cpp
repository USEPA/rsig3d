/****************************************************************************
** Meta object code from reading C++ file 'tonegeneratordialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tonegeneratordialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tonegeneratordialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToneGeneratorDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   21,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ToneGeneratorDialog[] = {
    "ToneGeneratorDialog\0\0enabled\0"
    "frequencySweepEnabled(bool)\0"
};

void ToneGeneratorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ToneGeneratorDialog *_t = static_cast<ToneGeneratorDialog *>(_o);
        switch (_id) {
        case 0: _t->frequencySweepEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ToneGeneratorDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ToneGeneratorDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ToneGeneratorDialog,
      qt_meta_data_ToneGeneratorDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToneGeneratorDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToneGeneratorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToneGeneratorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToneGeneratorDialog))
        return static_cast<void*>(const_cast< ToneGeneratorDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ToneGeneratorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
