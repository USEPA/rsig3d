/****************************************************************************
** Meta object code from reading C++ file 'downloadmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../downloadmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloadmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DownloadItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   13,   13,   13, 0x08,
      37,   13,   13,   13, 0x08,
      48,   13,   13,   13, 0x08,
      55,   13,   13,   13, 0x08,
      80,   75,   13,   13, 0x08,
     140,  115,   13,   13, 0x08,
     172,   13,   13,   13, 0x08,
     190,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DownloadItem[] = {
    "DownloadItem\0\0statusChanged()\0stop()\0"
    "tryAgain()\0open()\0downloadReadyRead()\0"
    "code\0error(QNetworkReply::NetworkError)\0"
    "bytesReceived,bytesTotal\0"
    "downloadProgress(qint64,qint64)\0"
    "metaDataChanged()\0finished()\0"
};

void DownloadItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DownloadItem *_t = static_cast<DownloadItem *>(_o);
        switch (_id) {
        case 0: _t->statusChanged(); break;
        case 1: _t->stop(); break;
        case 2: _t->tryAgain(); break;
        case 3: _t->open(); break;
        case 4: _t->downloadReadyRead(); break;
        case 5: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 6: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 7: _t->metaDataChanged(); break;
        case 8: _t->finished(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DownloadItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DownloadItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DownloadItem,
      qt_meta_data_DownloadItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DownloadItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DownloadItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DownloadItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadItem))
        return static_cast<void*>(const_cast< DownloadItem*>(this));
    if (!strcmp(_clname, "Ui_DownloadItem"))
        return static_cast< Ui_DownloadItem*>(const_cast< DownloadItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int DownloadItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DownloadItem::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_DownloadManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       1,   59, // properties
       1,   62, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      41,   17,   16,   16, 0x0a,
      80,   72,   16,   16, 0x2a,
     126,  106,   16,   16, 0x0a,
     150,  146,   16,   16, 0x2a,
     187,  165,   16,   16, 0x0a,
     239,  233,   16,   16, 0x2a,
     280,   16,   16,   16, 0x0a,
     290,   16,   16,   16, 0x08,
     297,   16,   16,   16, 0x08,

 // properties: name, type, flags
     322,  309, 0x0009510b,

 // enums: name, flags, count, data
     309, 0x0,    3,   66,

 // enum data: key, value
     335, uint(DownloadManager::Never),
     341, uint(DownloadManager::Exit),
     346, uint(DownloadManager::SuccessFullDownload),

       0        // eod
};

static const char qt_meta_stringdata_DownloadManager[] = {
    "DownloadManager\0\0request,requestFileName\0"
    "download(QNetworkRequest,bool)\0request\0"
    "download(QNetworkRequest)\0url,requestFileName\0"
    "download(QUrl,bool)\0url\0download(QUrl)\0"
    "reply,requestFileName\0"
    "handleUnsupportedContent(QNetworkReply*,bool)\0"
    "reply\0handleUnsupportedContent(QNetworkReply*)\0"
    "cleanup()\0save()\0updateRow()\0RemovePolicy\0"
    "removePolicy\0Never\0Exit\0SuccessFullDownload\0"
};

void DownloadManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DownloadManager *_t = static_cast<DownloadManager *>(_o);
        switch (_id) {
        case 0: _t->download((*reinterpret_cast< const QNetworkRequest(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->download((*reinterpret_cast< const QNetworkRequest(*)>(_a[1]))); break;
        case 2: _t->download((*reinterpret_cast< const QUrl(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->download((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 4: _t->handleUnsupportedContent((*reinterpret_cast< QNetworkReply*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->handleUnsupportedContent((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 6: _t->cleanup(); break;
        case 7: _t->save(); break;
        case 8: _t->updateRow(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DownloadManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DownloadManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DownloadManager,
      qt_meta_data_DownloadManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DownloadManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DownloadManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DownloadManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadManager))
        return static_cast<void*>(const_cast< DownloadManager*>(this));
    if (!strcmp(_clname, "Ui_DownloadDialog"))
        return static_cast< Ui_DownloadDialog*>(const_cast< DownloadManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int DownloadManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< RemovePolicy*>(_v) = removePolicy(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setRemovePolicy(*reinterpret_cast< RemovePolicy*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_DownloadModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_DownloadModel[] = {
    "DownloadModel\0"
};

void DownloadModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DownloadModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DownloadModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_DownloadModel,
      qt_meta_data_DownloadModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DownloadModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DownloadModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DownloadModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadModel))
        return static_cast<void*>(const_cast< DownloadModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int DownloadModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
