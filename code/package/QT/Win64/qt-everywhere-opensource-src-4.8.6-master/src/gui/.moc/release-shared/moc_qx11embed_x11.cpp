/****************************************************************************
** Meta object code from reading C++ file 'qx11embed_x11.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../kernel/qx11embed_x11.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qx11embed_x11.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QX11EmbedWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      28,   16,   16,   16, 0x05,
      52,   46,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QX11EmbedWidget[] = {
    "QX11EmbedWidget\0\0embedded()\0"
    "containerClosed()\0error\0"
    "error(QX11EmbedWidget::Error)\0"
};

void QX11EmbedWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QX11EmbedWidget *_t = static_cast<QX11EmbedWidget *>(_o);
        switch (_id) {
        case 0: _t->embedded(); break;
        case 1: _t->containerClosed(); break;
        case 2: _t->error((*reinterpret_cast< QX11EmbedWidget::Error(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QX11EmbedWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QX11EmbedWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QX11EmbedWidget,
      qt_meta_data_QX11EmbedWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QX11EmbedWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QX11EmbedWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QX11EmbedWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QX11EmbedWidget))
        return static_cast<void*>(const_cast< QX11EmbedWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QX11EmbedWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QX11EmbedWidget::embedded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QX11EmbedWidget::containerClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QX11EmbedWidget::error(QX11EmbedWidget::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_QX11EmbedContainer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      39,   19,   19,   19, 0x05,
      54,   19,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QX11EmbedContainer[] = {
    "QX11EmbedContainer\0\0clientIsEmbedded()\0"
    "clientClosed()\0error(QX11EmbedContainer::Error)\0"
};

void QX11EmbedContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QX11EmbedContainer *_t = static_cast<QX11EmbedContainer *>(_o);
        switch (_id) {
        case 0: _t->clientIsEmbedded(); break;
        case 1: _t->clientClosed(); break;
        case 2: _t->error((*reinterpret_cast< QX11EmbedContainer::Error(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QX11EmbedContainer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QX11EmbedContainer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QX11EmbedContainer,
      qt_meta_data_QX11EmbedContainer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QX11EmbedContainer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QX11EmbedContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QX11EmbedContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QX11EmbedContainer))
        return static_cast<void*>(const_cast< QX11EmbedContainer*>(this));
    return QWidget::qt_metacast(_clname);
}

int QX11EmbedContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QX11EmbedContainer::clientIsEmbedded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QX11EmbedContainer::clientClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QX11EmbedContainer::error(QX11EmbedContainer::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
