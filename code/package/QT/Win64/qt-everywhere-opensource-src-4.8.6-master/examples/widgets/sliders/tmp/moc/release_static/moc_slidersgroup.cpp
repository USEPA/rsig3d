/****************************************************************************
** Meta object code from reading C++ file 'slidersgroup.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../slidersgroup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slidersgroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SlidersGroup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   14,   13,   13, 0x0a,
      52,   14,   13,   13, 0x0a,
      68,   14,   13,   13, 0x0a,
      91,   84,   13,   13, 0x0a,
     114,   84,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SlidersGroup[] = {
    "SlidersGroup\0\0value\0valueChanged(int)\0"
    "setValue(int)\0setMinimum(int)\0"
    "setMaximum(int)\0invert\0invertAppearance(bool)\0"
    "invertKeyBindings(bool)\0"
};

void SlidersGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SlidersGroup *_t = static_cast<SlidersGroup *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setMinimum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setMaximum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->invertAppearance((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->invertKeyBindings((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SlidersGroup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SlidersGroup::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_SlidersGroup,
      qt_meta_data_SlidersGroup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SlidersGroup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SlidersGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SlidersGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SlidersGroup))
        return static_cast<void*>(const_cast< SlidersGroup*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int SlidersGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SlidersGroup::valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
