/****************************************************************************
** Meta object code from reading C++ file 'qtresourcemodel_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shared/qtresourcemodel_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtresourcemodel_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtResourceModel[] = {

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
      48,   17,   16,   16, 0x05,
      95,   90,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     130,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtResourceModel[] = {
    "QtResourceModel\0\0resourceSet,resourceSetChanged\0"
    "resourceSetActivated(QtResourceSet*,bool)\0"
    "path\0qrcFileModifiedExternally(QString)\0"
    "slotFileChanged(QString)\0"
};

void QtResourceModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtResourceModel *_t = static_cast<QtResourceModel *>(_o);
        switch (_id) {
        case 0: _t->resourceSetActivated((*reinterpret_cast< QtResourceSet*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->qrcFileModifiedExternally((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->d_func()->slotFileChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtResourceModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtResourceModel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtResourceModel,
      qt_meta_data_QtResourceModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtResourceModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtResourceModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtResourceModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtResourceModel))
        return static_cast<void*>(const_cast< QtResourceModel*>(this));
    return QObject::qt_metacast(_clname);
}

int QtResourceModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QtResourceModel::resourceSetActivated(QtResourceSet * _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QtResourceModel::qrcFileModifiedExternally(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
