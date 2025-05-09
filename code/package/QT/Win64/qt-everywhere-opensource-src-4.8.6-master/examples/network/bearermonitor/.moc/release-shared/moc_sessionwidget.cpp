/****************************************************************************
** Meta object code from reading C++ file 'sessionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sessionwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SessionWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      29,   14,   14,   14, 0x08,
      47,   14,   14,   14, 0x08,
      62,   14,   14,   14, 0x08,
      76,   14,   14,   14, 0x08,
      98,   92,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SessionWidget[] = {
    "SessionWidget\0\0openSession()\0"
    "openSyncSession()\0closeSession()\0"
    "stopSession()\0updateSession()\0error\0"
    "updateSessionError(QNetworkSession::SessionError)\0"
};

void SessionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SessionWidget *_t = static_cast<SessionWidget *>(_o);
        switch (_id) {
        case 0: _t->openSession(); break;
        case 1: _t->openSyncSession(); break;
        case 2: _t->closeSession(); break;
        case 3: _t->stopSession(); break;
        case 4: _t->updateSession(); break;
        case 5: _t->updateSessionError((*reinterpret_cast< QNetworkSession::SessionError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SessionWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SessionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SessionWidget,
      qt_meta_data_SessionWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SessionWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SessionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SessionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SessionWidget))
        return static_cast<void*>(const_cast< SessionWidget*>(this));
    if (!strcmp(_clname, "Ui_SessionWidget"))
        return static_cast< Ui_SessionWidget*>(const_cast< SessionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int SessionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
