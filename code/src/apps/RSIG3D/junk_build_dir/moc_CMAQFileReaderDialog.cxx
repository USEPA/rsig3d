/****************************************************************************
** Meta object code from reading C++ file 'CMAQFileReaderDialog.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CMAQFileReaderDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CMAQFileReaderDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CMAQFileReaderDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      32,   21,   21,   21, 0x08,
      43,   21,   21,   21, 0x08,
      55,   21,   21,   21, 0x08,
      67,   21,   21,   21, 0x08,
      93,   88,   21,   21, 0x08,
     146,   88,   21,   21, 0x08,
     187,   21,   21,   21, 0x08,
     206,   88,   21,   21, 0x08,
     257,   21,   21,   21, 0x08,
     276,   88,   21,   21, 0x08,
     327,   21,   21,   21, 0x08,
     349,   88,   21,   21, 0x08,
     403,   21,   21,   21, 0x08,
     420,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CMAQFileReaderDialog[] = {
    "CMAQFileReaderDialog\0\0onLocal()\0"
    "onRemote()\0onConnect()\0onSetUser()\0"
    "onSetDataDirectory()\0item\0"
    "onSelectedDataDirectoryListingItem(QListWidgetItem*)\0"
    "onSelectedDataVariable(QListWidgetItem*)\0"
    "onSetHTDirectory()\0"
    "onSelectedHTDirectoryListingItem(QListWidgetItem*)\0"
    "onSetZFDirectory()\0"
    "onSelectedZFDirectoryListingItem(QListWidgetItem*)\0"
    "onSetWWINDDirectory()\0"
    "onSelectedWWINDDirectoryListingItem(QListWidgetItem*)\0"
    "onReadVariable()\0onCancel()\0"
};

void CMAQFileReaderDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CMAQFileReaderDialog *_t = static_cast<CMAQFileReaderDialog *>(_o);
        switch (_id) {
        case 0: _t->onLocal(); break;
        case 1: _t->onRemote(); break;
        case 2: _t->onConnect(); break;
        case 3: _t->onSetUser(); break;
        case 4: _t->onSetDataDirectory(); break;
        case 5: _t->onSelectedDataDirectoryListingItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->onSelectedDataVariable((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->onSetHTDirectory(); break;
        case 8: _t->onSelectedHTDirectoryListingItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->onSetZFDirectory(); break;
        case 10: _t->onSelectedZFDirectoryListingItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->onSetWWINDDirectory(); break;
        case 12: _t->onSelectedWWINDDirectoryListingItem((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 13: _t->onReadVariable(); break;
        case 14: _t->onCancel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CMAQFileReaderDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CMAQFileReaderDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CMAQFileReaderDialog,
      qt_meta_data_CMAQFileReaderDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CMAQFileReaderDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CMAQFileReaderDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CMAQFileReaderDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CMAQFileReaderDialog))
        return static_cast<void*>(const_cast< CMAQFileReaderDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CMAQFileReaderDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
