/****************************************************************************
** Meta object code from reading C++ file 'multipagewidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../multipagewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multipagewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MultiPageWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       2,   49, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   17,   16,   16, 0x05,
      54,   48,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   80,   16,   16, 0x0a,
     114,  103,   16,   16, 0x0a,
     139,   17,   16,   16, 0x0a,
     164,  155,   16,   16, 0x0a,
     186,   17,   16,   16, 0x0a,

 // properties: name, type, flags
     211,  207, 0x02095103,
     232,  224, 0x0a085103,

       0        // eod
};

static const char qt_meta_stringdata_MultiPageWidget[] = {
    "MultiPageWidget\0\0index\0currentIndexChanged(int)\0"
    "title\0pageTitleChanged(QString)\0page\0"
    "addPage(QWidget*)\0index,page\0"
    "insertPage(int,QWidget*)\0removePage(int)\0"
    "newTitle\0setPageTitle(QString)\0"
    "setCurrentIndex(int)\0int\0currentIndex\0"
    "QString\0pageTitle\0"
};

void MultiPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MultiPageWidget *_t = static_cast<MultiPageWidget *>(_o);
        switch (_id) {
        case 0: _t->currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->pageTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->addPage((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 3: _t->insertPage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 4: _t->removePage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setPageTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->setCurrentIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MultiPageWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MultiPageWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MultiPageWidget,
      qt_meta_data_MultiPageWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MultiPageWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MultiPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MultiPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MultiPageWidget))
        return static_cast<void*>(const_cast< MultiPageWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MultiPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = currentIndex(); break;
        case 1: *reinterpret_cast< QString*>(_v) = pageTitle(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setCurrentIndex(*reinterpret_cast< int*>(_v)); break;
        case 1: setPageTitle(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MultiPageWidget::currentIndexChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiPageWidget::pageTitleChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
