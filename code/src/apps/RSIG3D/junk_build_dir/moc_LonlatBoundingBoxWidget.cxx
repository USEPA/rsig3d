/****************************************************************************
** Meta object code from reading C++ file 'LonlatBoundingBoxWidget.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LonlatBoundingBoxWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LonlatBoundingBoxWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LonlatBoundingBoxWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   55,   24,   24, 0x0a,
      93,   24,   24,   24, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_LonlatBoundingBoxWidget[] = {
    "LonlatBoundingBoxWidget\0\0"
    "boundingGeoBoxChanged(QRectF)\0rectf\0"
    "onBoundingGeoBoxChanged(QRectF)\0"
    "onBoundingGeoBoxChanged()\0"
};

void LonlatBoundingBoxWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LonlatBoundingBoxWidget *_t = static_cast<LonlatBoundingBoxWidget *>(_o);
        switch (_id) {
        case 0: _t->boundingGeoBoxChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 1: _t->onBoundingGeoBoxChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 2: _t->onBoundingGeoBoxChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LonlatBoundingBoxWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LonlatBoundingBoxWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LonlatBoundingBoxWidget,
      qt_meta_data_LonlatBoundingBoxWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LonlatBoundingBoxWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LonlatBoundingBoxWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LonlatBoundingBoxWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LonlatBoundingBoxWidget))
        return static_cast<void*>(const_cast< LonlatBoundingBoxWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int LonlatBoundingBoxWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void LonlatBoundingBoxWidget::boundingGeoBoxChanged(const QRectF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
