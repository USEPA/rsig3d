/****************************************************************************
** Meta object code from reading C++ file 'blockingclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../blockingclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'blockingclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlockingClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      44,   36,   15,   15, 0x08,
      85,   65,   15,   15, 0x08,
     111,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BlockingClient[] = {
    "BlockingClient\0\0requestNewFortune()\0"
    "fortune\0showFortune(QString)\0"
    "socketError,message\0displayError(int,QString)\0"
    "enableGetFortuneButton()\0"
};

void BlockingClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BlockingClient *_t = static_cast<BlockingClient *>(_o);
        switch (_id) {
        case 0: _t->requestNewFortune(); break;
        case 1: _t->showFortune((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->displayError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->enableGetFortuneButton(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BlockingClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BlockingClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BlockingClient,
      qt_meta_data_BlockingClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BlockingClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BlockingClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BlockingClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlockingClient))
        return static_cast<void*>(const_cast< BlockingClient*>(this));
    return QWidget::qt_metacast(_clname);
}

int BlockingClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
