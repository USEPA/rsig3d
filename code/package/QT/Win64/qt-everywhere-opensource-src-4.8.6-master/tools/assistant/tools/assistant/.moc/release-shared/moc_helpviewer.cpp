/****************************************************************************
** Meta object code from reading C++ file 'helpviewer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../helpviewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'helpviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HelpViewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      31,   27,   11,   11, 0x05,
      55,   51,   11,   11, 0x05,
      83,   75,   11,   11, 0x05,
     106,   75,   11,   11, 0x05,
     135,  130,   11,   11, 0x05,
     156,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     173,   11,   11,   11, 0x0a,
     180,   11,   11,   11, 0x0a,
     187,   11,   11,   11, 0x0a,
     197,   11,   11,   11, 0x0a,
     208,   11,   11,   11, 0x08,
     224,   11,   11,   11, 0x08,
     244,  241,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HelpViewer[] = {
    "HelpViewer\0\0titleChanged()\0yes\0"
    "copyAvailable(bool)\0url\0sourceChanged(QUrl)\0"
    "enabled\0forwardAvailable(bool)\0"
    "backwardAvailable(bool)\0link\0"
    "highlighted(QString)\0printRequested()\0"
    "copy()\0home()\0forward()\0backward()\0"
    "actionChanged()\0setLoadStarted()\0ok\0"
    "setLoadFinished(bool)\0"
};

void HelpViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HelpViewer *_t = static_cast<HelpViewer *>(_o);
        switch (_id) {
        case 0: _t->titleChanged(); break;
        case 1: _t->copyAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sourceChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 3: _t->forwardAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->backwardAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->highlighted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->printRequested(); break;
        case 7: _t->copy(); break;
        case 8: _t->home(); break;
        case 9: _t->forward(); break;
        case 10: _t->backward(); break;
        case 11: _t->actionChanged(); break;
        case 12: _t->setLoadStarted(); break;
        case 13: _t->setLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HelpViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HelpViewer::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_HelpViewer,
      qt_meta_data_HelpViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HelpViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HelpViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HelpViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HelpViewer))
        return static_cast<void*>(const_cast< HelpViewer*>(this));
    return QWebView::qt_metacast(_clname);
}

int HelpViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void HelpViewer::titleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void HelpViewer::copyAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HelpViewer::sourceChanged(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HelpViewer::forwardAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HelpViewer::backwardAvailable(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void HelpViewer::highlighted(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void HelpViewer::printRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
