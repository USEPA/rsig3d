/****************************************************************************
** Meta object code from reading C++ file 'torrentclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../torrentclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'torrentclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TorrentClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      62,   56,   14,   14, 0x05,
      90,   14,   14,   14, 0x05,
     110,   14,   14,   14, 0x05,
     142,  128,   14,   14, 0x05,
     172,  156,   14,   14, 0x05,
     206,  190,   14,   14, 0x05,
     242,  227,   14,   14, 0x05,
     267,  227,   14,   14, 0x05,
     290,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     300,   14,   14,   14, 0x0a,
     308,   14,   14,   14, 0x0a,
     322,  315,   14,   14, 0x0a,
     345,  338,   14,   14, 0x0a,
     392,  386,   14,   14, 0x09,
     446,  417,   14,   14, 0x08,
     481,   14,   14,   14, 0x08,
     518,  504,   14,   14, 0x08,
     542,   14,   14,   14, 0x08,
     560,   14,   14,   14, 0x08,
     597,   14,  577,   14, 0x08,
     616,   14,   14,   14, 0x08,
     642,  338,   14,   14, 0x08,
     680,   14,   14,   14, 0x08,
     702,  695,   14,   14, 0x08,
     757,  733,   14,   14, 0x08,
     810,  788,   14,   14, 0x08,
     850,  844,   14,   14, 0x08,
     879,  844,   14,   14, 0x08,
     919,  913,  909,   14, 0x08,
     959,   14,   14,   14, 0x08,
     977,   14,   14,   14, 0x08,
     997,  338,   14,   14, 0x08,
    1037,  338,   14,   14, 0x08,
    1089, 1066,  909,   14, 0x08,
    1138,   14,   14,   14, 0x08,
    1151,   14,   14,   14, 0x08,
    1175, 1166,   14,   14, 0x08,
    1209,   14,   14,   14, 0x08,
    1235, 1226,   14,   14, 0x08,
    1255,   14,   14,   14, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_TorrentClient[] = {
    "TorrentClient\0\0state\0"
    "stateChanged(TorrentClient::State)\0"
    "error\0error(TorrentClient::Error)\0"
    "downloadCompleted()\0peerInfoUpdated()\0"
    "uploadedBytes\0dataSent(int)\0downloadedBytes\0"
    "dataReceived(int)\0percentProgress\0"
    "progressUpdated(int)\0bytesPerSecond\0"
    "downloadRateUpdated(int)\0"
    "uploadRateUpdated(int)\0stopped()\0"
    "start()\0stop()\0paused\0setPaused(bool)\0"
    "client\0setupIncomingConnection(PeerWireClient*)\0"
    "event\0timerEvent(QTimerEvent*)\0"
    "readId,pieceIndex,begin,data\0"
    "sendToPeer(int,int,int,QByteArray)\0"
    "fullVerificationDone()\0pieceIndex,ok\0"
    "pieceVerified(int,bool)\0handleFileError()\0"
    "connectToPeers()\0QList<TorrentPeer*>\0"
    "weighedFreePeers()\0setupOutgoingConnection()\0"
    "initializeConnection(PeerWireClient*)\0"
    "removeClient()\0pieces\0"
    "peerPiecesAvailable(QBitArray)\0"
    "pieceIndex,begin,length\0"
    "peerRequestsBlock(int,int,int)\0"
    "pieceIndex,begin,data\0"
    "blockReceived(int,int,QByteArray)\0"
    "bytes\0peerWireBytesWritten(qint64)\0"
    "peerWireBytesReceived(qint64)\0int\0"
    "piece\0blocksLeftForPiece(const TorrentPiece*)\0"
    "scheduleUploads()\0scheduleDownloads()\0"
    "schedulePieceForClient(PeerWireClient*)\0"
    "requestMore(PeerWireClient*)\0"
    "client,piece,maxBlocks\0"
    "requestBlocks(PeerWireClient*,TorrentPiece*,int)\0"
    "peerChoked()\0peerUnchoked()\0peerList\0"
    "addToPeerList(QList<TorrentPeer>)\0"
    "trackerStopped()\0progress\0updateProgress(int)\0"
    "updateProgress()\0"
};

void TorrentClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TorrentClient *_t = static_cast<TorrentClient *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< TorrentClient::State(*)>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast< TorrentClient::Error(*)>(_a[1]))); break;
        case 2: _t->downloadCompleted(); break;
        case 3: _t->peerInfoUpdated(); break;
        case 4: _t->dataSent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->dataReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->progressUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->downloadRateUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->uploadRateUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->stopped(); break;
        case 10: _t->start(); break;
        case 11: _t->stop(); break;
        case 12: _t->setPaused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->setupIncomingConnection((*reinterpret_cast< PeerWireClient*(*)>(_a[1]))); break;
        case 14: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        case 15: _t->sendToPeer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 16: _t->fullVerificationDone(); break;
        case 17: _t->pieceVerified((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 18: _t->handleFileError(); break;
        case 19: _t->connectToPeers(); break;
        case 20: { QList<TorrentPeer*> _r = _t->weighedFreePeers();
            if (_a[0]) *reinterpret_cast< QList<TorrentPeer*>*>(_a[0]) = _r; }  break;
        case 21: _t->setupOutgoingConnection(); break;
        case 22: _t->initializeConnection((*reinterpret_cast< PeerWireClient*(*)>(_a[1]))); break;
        case 23: _t->removeClient(); break;
        case 24: _t->peerPiecesAvailable((*reinterpret_cast< const QBitArray(*)>(_a[1]))); break;
        case 25: _t->peerRequestsBlock((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 26: _t->blockReceived((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 27: _t->peerWireBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 28: _t->peerWireBytesReceived((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 29: { int _r = _t->blocksLeftForPiece((*reinterpret_cast< const TorrentPiece*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: _t->scheduleUploads(); break;
        case 31: _t->scheduleDownloads(); break;
        case 32: _t->schedulePieceForClient((*reinterpret_cast< PeerWireClient*(*)>(_a[1]))); break;
        case 33: _t->requestMore((*reinterpret_cast< PeerWireClient*(*)>(_a[1]))); break;
        case 34: { int _r = _t->requestBlocks((*reinterpret_cast< PeerWireClient*(*)>(_a[1])),(*reinterpret_cast< TorrentPiece*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 35: _t->peerChoked(); break;
        case 36: _t->peerUnchoked(); break;
        case 37: _t->addToPeerList((*reinterpret_cast< const QList<TorrentPeer>(*)>(_a[1]))); break;
        case 38: _t->trackerStopped(); break;
        case 39: _t->updateProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->updateProgress(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TorrentClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TorrentClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TorrentClient,
      qt_meta_data_TorrentClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TorrentClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TorrentClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TorrentClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TorrentClient))
        return static_cast<void*>(const_cast< TorrentClient*>(this));
    return QObject::qt_metacast(_clname);
}

int TorrentClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void TorrentClient::stateChanged(TorrentClient::State _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TorrentClient::error(TorrentClient::Error _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TorrentClient::downloadCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TorrentClient::peerInfoUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void TorrentClient::dataSent(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TorrentClient::dataReceived(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TorrentClient::progressUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TorrentClient::downloadRateUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void TorrentClient::uploadRateUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void TorrentClient::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
