/****************************************************************************
** Meta object code from reading C++ file 'globalactions.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../globalactions.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'globalactions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GlobalActions[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      41,   31,   14,   14, 0x0a,
      64,   14,   14,   14, 0x08,
      90,   14,   14,   14, 0x08,
     123,  116,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GlobalActions[] = {
    "GlobalActions\0\0updateActions()\0available\0"
    "setCopyAvailable(bool)\0slotAboutToShowBackMenu()\0"
    "slotAboutToShowNextMenu()\0action\0"
    "slotOpenActionUrl(QAction*)\0"
};

void GlobalActions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GlobalActions *_t = static_cast<GlobalActions *>(_o);
        switch (_id) {
        case 0: _t->updateActions(); break;
        case 1: _t->setCopyAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slotAboutToShowBackMenu(); break;
        case 3: _t->slotAboutToShowNextMenu(); break;
        case 4: _t->slotOpenActionUrl((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GlobalActions::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GlobalActions::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GlobalActions,
      qt_meta_data_GlobalActions, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GlobalActions::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GlobalActions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GlobalActions::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GlobalActions))
        return static_cast<void*>(const_cast< GlobalActions*>(this));
    return QObject::qt_metacast(_clname);
}

int GlobalActions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
