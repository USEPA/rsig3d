/****************************************************************************
** Meta object code from reading C++ file 'qdesigner_tabwidget_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shared/qdesigner_tabwidget_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdesigner_tabwidget_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTabWidgetEventFilter[] = {

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
      23,   22,   22,   22, 0x08,
      43,   22,   22,   22, 0x08,
      53,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QTabWidgetEventFilter[] = {
    "QTabWidgetEventFilter\0\0removeCurrentPage()\0"
    "addPage()\0addPageAfter()\0"
};

void QTabWidgetEventFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTabWidgetEventFilter *_t = static_cast<QTabWidgetEventFilter *>(_o);
        switch (_id) {
        case 0: _t->removeCurrentPage(); break;
        case 1: _t->addPage(); break;
        case 2: _t->addPageAfter(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QTabWidgetEventFilter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTabWidgetEventFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QTabWidgetEventFilter,
      qt_meta_data_QTabWidgetEventFilter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTabWidgetEventFilter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTabWidgetEventFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTabWidgetEventFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTabWidgetEventFilter))
        return static_cast<void*>(const_cast< QTabWidgetEventFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int QTabWidgetEventFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
