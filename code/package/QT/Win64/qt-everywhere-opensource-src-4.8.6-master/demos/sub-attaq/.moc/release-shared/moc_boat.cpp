/****************************************************************************
** Meta object code from reading C++ file 'boat.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../boat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'boat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Boat[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x05,
      22,    5,    5,    5, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Boat[] = {
    "Boat\0\0boatDestroyed()\0boatExecutionFinished()\0"
};

void Boat::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Boat *_t = static_cast<Boat *>(_o);
        switch (_id) {
        case 0: _t->boatDestroyed(); break;
        case 1: _t->boatExecutionFinished(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Boat::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Boat::staticMetaObject = {
    { &PixmapItem::staticMetaObject, qt_meta_stringdata_Boat,
      qt_meta_data_Boat, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Boat::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Boat::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Boat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Boat))
        return static_cast<void*>(const_cast< Boat*>(this));
    return PixmapItem::qt_metacast(_clname);
}

int Boat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PixmapItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Boat::boatDestroyed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Boat::boatExecutionFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
