/****************************************************************************
** Meta object code from reading C++ file 'sslclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sslclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sslclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      32,   10,   10,   10, 0x08,
      54,   48,   10,   10, 0x08,
     103,   10,   10,   10, 0x08,
     121,   10,   10,   10, 0x08,
     139,   10,   10,   10, 0x08,
     157,  150,   10,   10, 0x08,
     185,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SslClient[] = {
    "SslClient\0\0updateEnabledState()\0"
    "secureConnect()\0state\0"
    "socketStateChanged(QAbstractSocket::SocketState)\0"
    "socketEncrypted()\0socketReadyRead()\0"
    "sendData()\0errors\0sslErrors(QList<QSslError>)\0"
    "displayCertificateInfo()\0"
};

void SslClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SslClient *_t = static_cast<SslClient *>(_o);
        switch (_id) {
        case 0: _t->updateEnabledState(); break;
        case 1: _t->secureConnect(); break;
        case 2: _t->socketStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 3: _t->socketEncrypted(); break;
        case 4: _t->socketReadyRead(); break;
        case 5: _t->sendData(); break;
        case 6: _t->sslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 7: _t->displayCertificateInfo(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SslClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SslClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SslClient,
      qt_meta_data_SslClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslClient))
        return static_cast<void*>(const_cast< SslClient*>(this));
    return QWidget::qt_metacast(_clname);
}

int SslClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
