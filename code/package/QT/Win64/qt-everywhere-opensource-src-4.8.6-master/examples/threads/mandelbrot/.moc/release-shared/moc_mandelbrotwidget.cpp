/****************************************************************************
** Meta object code from reading C++ file 'mandelbrotwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mandelbrotwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mandelbrotwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MandelbrotWidget[] = {

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
      36,   18,   17,   17, 0x08,
      75,   64,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MandelbrotWidget[] = {
    "MandelbrotWidget\0\0image,scaleFactor\0"
    "updatePixmap(QImage,double)\0zoomFactor\0"
    "zoom(double)\0"
};

void MandelbrotWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MandelbrotWidget *_t = static_cast<MandelbrotWidget *>(_o);
        switch (_id) {
        case 0: _t->updatePixmap((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->zoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MandelbrotWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MandelbrotWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MandelbrotWidget,
      qt_meta_data_MandelbrotWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MandelbrotWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MandelbrotWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MandelbrotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MandelbrotWidget))
        return static_cast<void*>(const_cast< MandelbrotWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MandelbrotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
