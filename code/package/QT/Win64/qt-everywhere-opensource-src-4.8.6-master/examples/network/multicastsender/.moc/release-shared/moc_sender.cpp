/****************************************************************************
** Meta object code from reading C++ file 'sender.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sender.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Sender[] = {

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
      15,    8,    7,    7, 0x08,
      31,    7,    7,    7, 0x08,
      46,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Sender[] = {
    "Sender\0\0newTtl\0ttlChanged(int)\0"
    "startSending()\0sendDatagram()\0"
};

void Sender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Sender *_t = static_cast<Sender *>(_o);
        switch (_id) {
        case 0: _t->ttlChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->startSending(); break;
        case 2: _t->sendDatagram(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Sender::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Sender::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Sender,
      qt_meta_data_Sender, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Sender::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Sender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Sender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Sender))
        return static_cast<void*>(const_cast< Sender*>(this));
    return QDialog::qt_metacast(_clname);
}

int Sender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
