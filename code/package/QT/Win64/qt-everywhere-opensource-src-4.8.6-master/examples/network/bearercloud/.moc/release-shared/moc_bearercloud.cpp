/****************************************************************************
** Meta object code from reading C++ file 'bearercloud.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../bearercloud.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bearercloud.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BearerCloud[] = {

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
      20,   13,   12,   12, 0x08,
      62,   13,   12,   12, 0x08,
     106,   13,   12,   12, 0x08,
     150,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BearerCloud[] = {
    "BearerCloud\0\0config\0"
    "configurationAdded(QNetworkConfiguration)\0"
    "configurationRemoved(QNetworkConfiguration)\0"
    "configurationChanged(QNetworkConfiguration)\0"
    "updateConfigurations()\0"
};

void BearerCloud::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BearerCloud *_t = static_cast<BearerCloud *>(_o);
        switch (_id) {
        case 0: _t->configurationAdded((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 1: _t->configurationRemoved((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 2: _t->configurationChanged((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 3: _t->updateConfigurations(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BearerCloud::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BearerCloud::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_BearerCloud,
      qt_meta_data_BearerCloud, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BearerCloud::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BearerCloud::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BearerCloud::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BearerCloud))
        return static_cast<void*>(const_cast< BearerCloud*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int BearerCloud::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
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
