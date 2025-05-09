/****************************************************************************
** Meta object code from reading C++ file 'peermanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../peermanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'peermanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PeerManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   12,   12,   12, 0x08,
      75,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PeerManager[] = {
    "PeerManager\0\0connection\0"
    "newConnection(Connection*)\0"
    "sendBroadcastDatagram()\0readBroadcastDatagram()\0"
};

void PeerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PeerManager *_t = static_cast<PeerManager *>(_o);
        switch (_id) {
        case 0: _t->newConnection((*reinterpret_cast< Connection*(*)>(_a[1]))); break;
        case 1: _t->sendBroadcastDatagram(); break;
        case 2: _t->readBroadcastDatagram(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PeerManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PeerManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PeerManager,
      qt_meta_data_PeerManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PeerManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PeerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PeerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PeerManager))
        return static_cast<void*>(const_cast< PeerManager*>(this));
    return QObject::qt_metacast(_clname);
}

int PeerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PeerManager::newConnection(Connection * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
