/****************************************************************************
** Meta object code from reading C++ file 'toplevel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../toplevel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'toplevel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KAstTopLevel[] = {

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
      14,   13,   13,   13, 0x08,
      28,   13,   13,   13, 0x08,
      50,   45,   13,   13, 0x08,
      67,   13,   13,   13, 0x08,
      86,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KAstTopLevel[] = {
    "KAstTopLevel\0\0slotNewGame()\0"
    "slotShipKilled()\0size\0slotRockHit(int)\0"
    "slotRocksRemoved()\0slotUpdateVitals()\0"
};

void KAstTopLevel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KAstTopLevel *_t = static_cast<KAstTopLevel *>(_o);
        switch (_id) {
        case 0: _t->slotNewGame(); break;
        case 1: _t->slotShipKilled(); break;
        case 2: _t->slotRockHit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotRocksRemoved(); break;
        case 4: _t->slotUpdateVitals(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData KAstTopLevel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject KAstTopLevel::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_KAstTopLevel,
      qt_meta_data_KAstTopLevel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KAstTopLevel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KAstTopLevel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KAstTopLevel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KAstTopLevel))
        return static_cast<void*>(const_cast< KAstTopLevel*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int KAstTopLevel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
