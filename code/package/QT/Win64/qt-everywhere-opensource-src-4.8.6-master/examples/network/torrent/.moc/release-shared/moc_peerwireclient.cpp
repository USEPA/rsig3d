/****************************************************************************
** Meta object code from reading C++ file 'peerwireclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../peerwireclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'peerwireclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PeerWireClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   16,   15,   15, 0x05,
      54,   15,   15,   15, 0x05,
      72,   15,   15,   15, 0x05,
      81,   15,   15,   15, 0x05,
      92,   15,   15,   15, 0x05,
     105,   15,   15,   15, 0x05,
     128,  121,   15,   15, 0x05,
     179,  155,   15,   15, 0x05,
     229,  207,   15,   15, 0x05,
     268,  263,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     313,  290,   15,   15, 0x09,
     381,  367,   15,   15, 0x29,
     426,   15,   15,   15, 0x09,
     460,   15,   15,   15, 0x08,
     476,   15,   15,   15, 0x08,
     504,  498,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PeerWireClient[] = {
    "PeerWireClient\0\0infoHash\0"
    "infoHashReceived(QByteArray)\0"
    "readyToTransfer()\0choked()\0unchoked()\0"
    "interested()\0notInterested()\0pieces\0"
    "piecesAvailable(QBitArray)\0"
    "pieceIndex,begin,length\0"
    "blockRequested(int,int,int)\0"
    "pieceIndex,begin,data\0"
    "blockReceived(int,int,QByteArray)\0"
    "size\0bytesReceived(qint64)\0"
    "hostName,port,openMode\0"
    "connectToHostImplementation(QString,quint16,OpenMode)\0"
    "hostName,port\0"
    "connectToHostImplementation(QString,quint16)\0"
    "diconnectFromHostImplementation()\0"
    "sendHandShake()\0processIncomingData()\0"
    "state\0socketStateChanged(QAbstractSocket::SocketState)\0"
};

void PeerWireClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PeerWireClient *_t = static_cast<PeerWireClient *>(_o);
        switch (_id) {
        case 0: _t->infoHashReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->readyToTransfer(); break;
        case 2: _t->choked(); break;
        case 3: _t->unchoked(); break;
        case 4: _t->interested(); break;
        case 5: _t->notInterested(); break;
        case 6: _t->piecesAvailable((*reinterpret_cast< const QBitArray(*)>(_a[1]))); break;
        case 7: _t->blockRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->blockReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 9: _t->bytesReceived((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 10: _t->connectToHostImplementation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< OpenMode(*)>(_a[3]))); break;
        case 11: _t->connectToHostImplementation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 12: _t->diconnectFromHostImplementation(); break;
        case 13: _t->sendHandShake(); break;
        case 14: _t->processIncomingData(); break;
        case 15: _t->socketStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PeerWireClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PeerWireClient::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_PeerWireClient,
      qt_meta_data_PeerWireClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PeerWireClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PeerWireClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PeerWireClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PeerWireClient))
        return static_cast<void*>(const_cast< PeerWireClient*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int PeerWireClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void PeerWireClient::infoHashReceived(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PeerWireClient::readyToTransfer()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void PeerWireClient::choked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void PeerWireClient::unchoked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void PeerWireClient::interested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void PeerWireClient::notInterested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void PeerWireClient::piecesAvailable(const QBitArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void PeerWireClient::blockRequested(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void PeerWireClient::blockReceived(int _t1, int _t2, const QByteArray & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void PeerWireClient::bytesReceived(qint64 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_END_MOC_NAMESPACE
