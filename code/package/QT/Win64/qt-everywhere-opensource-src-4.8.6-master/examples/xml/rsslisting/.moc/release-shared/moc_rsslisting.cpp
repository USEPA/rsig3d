/****************************************************************************
** Meta object code from reading C++ file 'rsslisting.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../rsslisting.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rsslisting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RSSListing[] = {

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
      12,   11,   11,   11, 0x0a,
      26,   20,   11,   11, 0x0a,
      51,   11,   11,   11, 0x0a,
      63,   11,   11,   11, 0x0a,
      86,   81,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RSSListing[] = {
    "RSSListing\0\0fetch()\0reply\0"
    "finished(QNetworkReply*)\0readyRead()\0"
    "metaDataChanged()\0item\0"
    "itemActivated(QTreeWidgetItem*)\0"
    "error(QNetworkReply::NetworkError)\0"
};

void RSSListing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RSSListing *_t = static_cast<RSSListing *>(_o);
        switch (_id) {
        case 0: _t->fetch(); break;
        case 1: _t->finished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 2: _t->readyRead(); break;
        case 3: _t->metaDataChanged(); break;
        case 4: _t->itemActivated((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RSSListing::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RSSListing::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RSSListing,
      qt_meta_data_RSSListing, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RSSListing::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RSSListing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RSSListing::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RSSListing))
        return static_cast<void*>(const_cast< RSSListing*>(this));
    return QWidget::qt_metacast(_clname);
}

int RSSListing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
