/****************************************************************************
** Meta object code from reading C++ file 'qtgradientmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../shared/qtgradienteditor/qtgradientmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtgradientmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtGradientManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x05,
      73,   64,   18,   18, 0x05,
     121,  106,   18,   18, 0x05,
     159,  156,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     192,   19,  184,   18, 0x0a,
     223,   64,   18,   18, 0x0a,
     255,  106,   18,   18, 0x0a,
     289,  156,   18,   18, 0x0a,
     313,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtGradientManager[] = {
    "QtGradientManager\0\0id,gradient\0"
    "gradientAdded(QString,QGradient)\0"
    "id,newId\0gradientRenamed(QString,QString)\0"
    "id,newGradient\0gradientChanged(QString,QGradient)\0"
    "id\0gradientRemoved(QString)\0QString\0"
    "addGradient(QString,QGradient)\0"
    "renameGradient(QString,QString)\0"
    "changeGradient(QString,QGradient)\0"
    "removeGradient(QString)\0clear()\0"
};

void QtGradientManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtGradientManager *_t = static_cast<QtGradientManager *>(_o);
        switch (_id) {
        case 0: _t->gradientAdded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QGradient(*)>(_a[2]))); break;
        case 1: _t->gradientRenamed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->gradientChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QGradient(*)>(_a[2]))); break;
        case 3: _t->gradientRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: { QString _r = _t->addGradient((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QGradient(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: _t->renameGradient((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->changeGradient((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QGradient(*)>(_a[2]))); break;
        case 7: _t->removeGradient((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->clear(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtGradientManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtGradientManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtGradientManager,
      qt_meta_data_QtGradientManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtGradientManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtGradientManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtGradientManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtGradientManager))
        return static_cast<void*>(const_cast< QtGradientManager*>(this));
    return QObject::qt_metacast(_clname);
}

int QtGradientManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QtGradientManager::gradientAdded(const QString & _t1, const QGradient & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtGradientManager::gradientRenamed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QtGradientManager::gradientChanged(const QString & _t1, const QGradient & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QtGradientManager::gradientRemoved(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
