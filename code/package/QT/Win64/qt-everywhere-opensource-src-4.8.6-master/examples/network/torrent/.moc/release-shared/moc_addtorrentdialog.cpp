/****************************************************************************
** Meta object code from reading C++ file 'addtorrentdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../addtorrentdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'addtorrentdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AddTorrentDialog[] = {

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
      30,   18,   17,   17, 0x0a,
      50,   17,   17,   17, 0x08,
      66,   17,   17,   17, 0x08,
      86,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AddTorrentDialog[] = {
    "AddTorrentDialog\0\0torrentFile\0"
    "setTorrent(QString)\0selectTorrent()\0"
    "selectDestination()\0enableOkButton()\0"
};

void AddTorrentDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AddTorrentDialog *_t = static_cast<AddTorrentDialog *>(_o);
        switch (_id) {
        case 0: _t->setTorrent((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->selectTorrent(); break;
        case 2: _t->selectDestination(); break;
        case 3: _t->enableOkButton(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AddTorrentDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AddTorrentDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AddTorrentDialog,
      qt_meta_data_AddTorrentDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AddTorrentDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AddTorrentDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AddTorrentDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AddTorrentDialog))
        return static_cast<void*>(const_cast< AddTorrentDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int AddTorrentDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
