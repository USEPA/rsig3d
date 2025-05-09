/****************************************************************************
** Meta object code from reading C++ file 'ftpreply.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ftpreply.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpreply.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FtpReply[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   10,    9,    9, 0x08,
      57,   49,    9,    9, 0x08,
      83,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FtpReply[] = {
    "FtpReply\0\0command,error\0"
    "processCommand(int,bool)\0urlInfo\0"
    "processListInfo(QUrlInfo)\0processData()\0"
};

void FtpReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FtpReply *_t = static_cast<FtpReply *>(_o);
        switch (_id) {
        case 0: _t->processCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->processListInfo((*reinterpret_cast< const QUrlInfo(*)>(_a[1]))); break;
        case 2: _t->processData(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FtpReply::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FtpReply::staticMetaObject = {
    { &QNetworkReply::staticMetaObject, qt_meta_stringdata_FtpReply,
      qt_meta_data_FtpReply, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FtpReply::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FtpReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FtpReply::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FtpReply))
        return static_cast<void*>(const_cast< FtpReply*>(this));
    return QNetworkReply::qt_metacast(_clname);
}

int FtpReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QNetworkReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
