/****************************************************************************
** Meta object code from reading C++ file 'settingstree.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../settingstree.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingstree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingsTree[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   14,   13,   13, 0x0a,
      55,   47,   13,   13, 0x0a,
      81,   13,   13,   13, 0x0a,
      96,   13,   13,   13, 0x0a,
     111,  106,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SettingsTree[] = {
    "SettingsTree\0\0autoRefresh\0"
    "setAutoRefresh(bool)\0enabled\0"
    "setFallbacksEnabled(bool)\0maybeRefresh()\0"
    "refresh()\0item\0updateSetting(QTreeWidgetItem*)\0"
};

void SettingsTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SettingsTree *_t = static_cast<SettingsTree *>(_o);
        switch (_id) {
        case 0: _t->setAutoRefresh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setFallbacksEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->maybeRefresh(); break;
        case 3: _t->refresh(); break;
        case 4: _t->updateSetting((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SettingsTree::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SettingsTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_SettingsTree,
      qt_meta_data_SettingsTree, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingsTree::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingsTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingsTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsTree))
        return static_cast<void*>(const_cast< SettingsTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int SettingsTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
