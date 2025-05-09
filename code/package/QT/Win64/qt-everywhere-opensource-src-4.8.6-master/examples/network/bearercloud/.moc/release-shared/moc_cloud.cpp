/****************************************************************************
** Meta object code from reading C++ file 'cloud.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cloud.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cloud.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Cloud[] = {

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
      13,    7,    6,    6, 0x08,
      50,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Cloud[] = {
    "Cloud\0\0state\0stateChanged(QNetworkSession::State)\0"
    "newConfigurationActivated()\0"
};

void Cloud::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Cloud *_t = static_cast<Cloud *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< QNetworkSession::State(*)>(_a[1]))); break;
        case 1: _t->newConfigurationActivated(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Cloud::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Cloud::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cloud,
      qt_meta_data_Cloud, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Cloud::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Cloud::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Cloud::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Cloud))
        return static_cast<void*>(const_cast< Cloud*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Cloud*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Cloud*>(this));
    return QObject::qt_metacast(_clname);
}

int Cloud::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
