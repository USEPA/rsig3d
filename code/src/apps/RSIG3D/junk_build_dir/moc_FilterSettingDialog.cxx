/****************************************************************************
** Meta object code from reading C++ file 'FilterSettingDialog.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FilterSettingDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FilterSettingDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FilterSettingDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      28,   20,   20,   20, 0x0a,
      39,   20,   20,   20, 0x0a,
      49,   20,   20,   20, 0x0a,
      76,   20,   20,   20, 0x0a,
      96,   20,   20,   20, 0x0a,
     121,   20,  116,   20, 0x0a,
     144,   20,  116,   20, 0x0a,
     163,   20,  116,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FilterSettingDialog[] = {
    "FilterSettingDialog\0\0onOK()\0onCancel()\0"
    "onReset()\0writePurpleairApiKeyFile()\0"
    "writeTEMPOKeyFile()\0writeOAQPSKeyFile()\0"
    "bool\0validatePurpleairKey()\0"
    "validateTEMPOKey()\0validateOAQPSKey()\0"
};

void FilterSettingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FilterSettingDialog *_t = static_cast<FilterSettingDialog *>(_o);
        switch (_id) {
        case 0: _t->onOK(); break;
        case 1: _t->onCancel(); break;
        case 2: _t->onReset(); break;
        case 3: _t->writePurpleairApiKeyFile(); break;
        case 4: _t->writeTEMPOKeyFile(); break;
        case 5: _t->writeOAQPSKeyFile(); break;
        case 6: { bool _r = _t->validatePurpleairKey();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->validateTEMPOKey();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->validateOAQPSKey();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FilterSettingDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FilterSettingDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FilterSettingDialog,
      qt_meta_data_FilterSettingDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FilterSettingDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FilterSettingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FilterSettingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FilterSettingDialog))
        return static_cast<void*>(const_cast< FilterSettingDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FilterSettingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
