/****************************************************************************
** Meta object code from reading C++ file 'HttpDownloadDialog.h'
**
** Created: Thu May 8 12:58:26 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../HttpDownloadDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HttpDownloadDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataReader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataReader[] = {
    "DataReader\0\0finished(QObject*)\0start()\0"
};

void DataReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataReader *_t = static_cast<DataReader *>(_o);
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->start(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataReader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataReader,
      qt_meta_data_DataReader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataReader))
        return static_cast<void*>(const_cast< DataReader*>(this));
    return QObject::qt_metacast(_clname);
}

int DataReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DataReader::finished(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_LibCurlDataGet[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LibCurlDataGet[] = {
    "LibCurlDataGet\0\0data\0finished(QObject*)\0"
    "start()\0"
};

void LibCurlDataGet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LibCurlDataGet *_t = static_cast<LibCurlDataGet *>(_o);
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->start(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LibCurlDataGet::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LibCurlDataGet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LibCurlDataGet,
      qt_meta_data_LibCurlDataGet, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LibCurlDataGet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LibCurlDataGet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LibCurlDataGet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LibCurlDataGet))
        return static_cast<void*>(const_cast< LibCurlDataGet*>(this));
    return QObject::qt_metacast(_clname);
}

int LibCurlDataGet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void LibCurlDataGet::finished(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_HttpDownloadDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   20,   19,   19, 0x05,
      48,   19,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   19,   19,   19, 0x0a,
      71,   69,   19,   19, 0x0a,
      97,   20,   19,   19, 0x0a,
     125,  119,   19,   19, 0x09,
     195,  170,   19,   19, 0x09,
     239,  229,   19,   19, 0x09,
     283,  276,   19,   19, 0x09,
     313,   19,   19,   19, 0x09,
     332,   19,   19,   19, 0x09,
     339,   19,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_HttpDownloadDialog[] = {
    "HttpDownloadDialog\0\0data\0"
    "workFinished(QObject*)\0workAborted()\0"
    "show()\0e\0keyPressEvent(QKeyEvent*)\0"
    "onDataReady(QObject*)\0reply\0"
    "onHttpDownloadDialogFinished(QNetworkReply*)\0"
    "bytesReceived,bytesTotal\0"
    "onDownloadProgress(qint64,qint64)\0"
    "errorcode\0onError(QNetworkReply::NetworkError)\0"
    "errors\0onSslErrors(QList<QSslError>)\0"
    "onShowlogClicked()\0onOK()\0onCancel()\0"
};

void HttpDownloadDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HttpDownloadDialog *_t = static_cast<HttpDownloadDialog *>(_o);
        switch (_id) {
        case 0: _t->workFinished((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->workAborted(); break;
        case 2: _t->show(); break;
        case 3: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 4: _t->onDataReady((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 5: _t->onHttpDownloadDialogFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 6: _t->onDownloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 7: _t->onError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 8: _t->onSslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 9: _t->onShowlogClicked(); break;
        case 10: _t->onOK(); break;
        case 11: _t->onCancel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HttpDownloadDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HttpDownloadDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_HttpDownloadDialog,
      qt_meta_data_HttpDownloadDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HttpDownloadDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HttpDownloadDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HttpDownloadDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HttpDownloadDialog))
        return static_cast<void*>(const_cast< HttpDownloadDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int HttpDownloadDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void HttpDownloadDialog::workFinished(QObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HttpDownloadDialog::workAborted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
