/****************************************************************************
** Meta object code from reading C++ file 'imagescaling.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imagescaling.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagescaling.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Images[] = {

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
       8,    7,    7,    7, 0x0a,
      19,   15,    7,    7, 0x0a,
      34,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Images[] = {
    "Images\0\0open()\0num\0showImage(int)\0"
    "finished()\0"
};

void Images::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Images *_t = static_cast<Images *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->showImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->finished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Images::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Images::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Images,
      qt_meta_data_Images, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Images::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Images::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Images::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Images))
        return static_cast<void*>(const_cast< Images*>(this));
    return QWidget::qt_metacast(_clname);
}

int Images::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
