/****************************************************************************
** Meta object code from reading C++ file 'RSIG3DHelp.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../RSIG3DHelp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RSIG3DHelp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RSIG3DHelp[] = {

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
      12,   11,   11,   11, 0x09,
      23,   19,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_RSIG3DHelp[] = {
    "RSIG3DHelp\0\0onOk()\0val\0"
    "onDontShowAgainClicked(int)\0"
};

void RSIG3DHelp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RSIG3DHelp *_t = static_cast<RSIG3DHelp *>(_o);
        switch (_id) {
        case 0: _t->onOk(); break;
        case 1: _t->onDontShowAgainClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RSIG3DHelp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RSIG3DHelp::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_RSIG3DHelp,
      qt_meta_data_RSIG3DHelp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RSIG3DHelp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RSIG3DHelp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RSIG3DHelp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RSIG3DHelp))
        return static_cast<void*>(const_cast< RSIG3DHelp*>(this));
    return QDialog::qt_metacast(_clname);
}

int RSIG3DHelp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
