/****************************************************************************
** Meta object code from reading C++ file 'VisDataList.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VisDataList.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisDataList.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VisDataList[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
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

       0        // eod
};

static const char qt_meta_stringdata_VisDataList[] = {
    "VisDataList\0\0obj\0objectAdded(QObject*)\0"
    "objectRemoved(QObject*)\0"
    "onObjectRemoved(QObject*)\0"
};

void VisDataList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VisDataList *_t = static_cast<VisDataList *>(_o);
        switch (_id) {
        case 0: _t->objectAdded((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->objectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 2: _t->onObjectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VisDataList::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VisDataList::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VisDataList,
      qt_meta_data_VisDataList, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VisDataList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VisDataList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VisDataList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VisDataList))
        return static_cast<void*>(const_cast< VisDataList*>(this));
    return QObject::qt_metacast(_clname);
}

int VisDataList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void VisDataList::objectAdded(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VisDataList::objectRemoved(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
