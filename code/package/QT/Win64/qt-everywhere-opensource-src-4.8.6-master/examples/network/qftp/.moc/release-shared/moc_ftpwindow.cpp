/****************************************************************************
** Meta object code from reading C++ file 'ftpwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ftpwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FtpWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      33,   10,   10,   10, 0x08,
      48,   10,   10,   10, 0x08,
      65,   10,   10,   10, 0x08,
      96,   80,   10,   10, 0x08,
     133,  125,   10,   10, 0x08,
     165,  153,   10,   10, 0x08,
     199,   10,   10,   10, 0x08,
     233,  212,   10,   10, 0x08,
     275,   10,   10,   10, 0x08,
     298,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FtpWindow[] = {
    "FtpWindow\0\0connectOrDisconnect()\0"
    "downloadFile()\0cancelDownload()\0"
    "connectToFtp()\0commandId,error\0"
    "ftpCommandFinished(int,bool)\0urlInfo\0"
    "addToList(QUrlInfo)\0item,column\0"
    "processItem(QTreeWidgetItem*,int)\0"
    "cdToParent()\0readBytes,totalBytes\0"
    "updateDataTransferProgress(qint64,qint64)\0"
    "enableDownloadButton()\0enableConnectButton()\0"
};

void FtpWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FtpWindow *_t = static_cast<FtpWindow *>(_o);
        switch (_id) {
        case 0: _t->connectOrDisconnect(); break;
        case 1: _t->downloadFile(); break;
        case 2: _t->cancelDownload(); break;
        case 3: _t->connectToFtp(); break;
        case 4: _t->ftpCommandFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->addToList((*reinterpret_cast< const QUrlInfo(*)>(_a[1]))); break;
        case 6: _t->processItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->cdToParent(); break;
        case 8: _t->updateDataTransferProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 9: _t->enableDownloadButton(); break;
        case 10: _t->enableConnectButton(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FtpWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FtpWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FtpWindow,
      qt_meta_data_FtpWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FtpWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FtpWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FtpWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FtpWindow))
        return static_cast<void*>(const_cast< FtpWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int FtpWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
