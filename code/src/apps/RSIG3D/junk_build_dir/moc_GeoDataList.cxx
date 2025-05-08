/****************************************************************************
** Meta object code from reading C++ file 'GeoDataList.h'
**
** Created: Thu May 8 12:58:25 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GeoDataList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GeoDataList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GeoDataList[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,
      39,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   13,   12,   12, 0x0a,
      95,   89,   12,   12, 0x0a,
     116,   13,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GeoDataList[] = {
    "GeoDataList\0\0obj\0objectAdded(QObject*)\0"
    "objectRemoved(QObject*)\0"
    "onObjectRemoved(QObject*)\0objID\0"
    "onObjectRemoved(int)\0"
    "onChildObjectRemoved(QObject*)\0"
};

void GeoDataList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GeoDataList *_t = static_cast<GeoDataList *>(_o);
        switch (_id) {
        case 0: _t->objectAdded((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->objectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 2: _t->onObjectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 3: _t->onObjectRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onChildObjectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GeoDataList::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GeoDataList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GeoDataList,
      qt_meta_data_GeoDataList, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GeoDataList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GeoDataList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GeoDataList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeoDataList))
        return static_cast<void*>(const_cast< GeoDataList*>(this));
    return QObject::qt_metacast(_clname);
}

int GeoDataList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GeoDataList::objectAdded(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GeoDataList::objectRemoved(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
