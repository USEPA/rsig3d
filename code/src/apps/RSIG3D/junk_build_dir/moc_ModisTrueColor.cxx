/****************************************************************************
** Meta object code from reading C++ file 'ModisTrueColor.h'
**
** Created: Thu May 8 12:58:25 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ModisTrueColor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModisTrueColor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModisTrueColor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      41,   27,   15,   15, 0x05,
      56,   27,   15,   15, 0x05,
      73,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      94,   15,   89,   15, 0x0a,
     110,   15,   15,   15, 0x0a,
     124,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ModisTrueColor[] = {
    "ModisTrueColor\0\0finished()\0messageString\0"
    "error(QString)\0message(QString)\0"
    "requestRedraw()\0bool\0downloadImage()\0"
    "deleteLater()\0done()\0"
};

void ModisTrueColor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModisTrueColor *_t = static_cast<ModisTrueColor *>(_o);
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->requestRedraw(); break;
        case 4: { bool _r = _t->downloadImage();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: _t->deleteLater(); break;
        case 6: _t->done(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModisTrueColor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModisTrueColor::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ModisTrueColor,
      qt_meta_data_ModisTrueColor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModisTrueColor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModisTrueColor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModisTrueColor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModisTrueColor))
        return static_cast<void*>(const_cast< ModisTrueColor*>(this));
    return QThread::qt_metacast(_clname);
}

int ModisTrueColor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ModisTrueColor::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ModisTrueColor::error(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ModisTrueColor::message(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ModisTrueColor::requestRedraw()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
