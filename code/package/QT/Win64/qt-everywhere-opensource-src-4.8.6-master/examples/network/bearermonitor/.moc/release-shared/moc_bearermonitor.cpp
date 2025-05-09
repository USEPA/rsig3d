/****************************************************************************
** Meta object code from reading C++ file 'bearermonitor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../bearermonitor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bearermonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BearerMonitor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   15,   14,   14, 0x08,
      95,   88,   14,   14, 0x28,
     137,   88,   14,   14, 0x08,
     181,   88,   14,   14, 0x08,
     237,  225,   14,   14, 0x08,
     301,   14,   14,   14, 0x08,
     333,  324,   14,   14, 0x08,
     363,  358,   14,   14, 0x08,
     402,  358,   14,   14, 0x08,
     437,   14,   14,   14, 0x08,
     456,   14,   14,   14, 0x08,
     472,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BearerMonitor[] = {
    "BearerMonitor\0\0config,parent\0"
    "configurationAdded(QNetworkConfiguration,QTreeWidgetItem*)\0"
    "config\0configurationAdded(QNetworkConfiguration)\0"
    "configurationRemoved(QNetworkConfiguration)\0"
    "configurationChanged(QNetworkConfiguration)\0"
    "parent,snap\0"
    "updateSnapConfiguration(QTreeWidgetItem*,QNetworkConfiguration)\0"
    "updateConfigurations()\0isOnline\0"
    "onlineStateChanged(bool)\0item\0"
    "showConfigurationFor(QTreeWidgetItem*)\0"
    "createSessionFor(QTreeWidgetItem*)\0"
    "createNewSession()\0deleteSession()\0"
    "performScan()\0"
};

void BearerMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BearerMonitor *_t = static_cast<BearerMonitor *>(_o);
        switch (_id) {
        case 0: _t->configurationAdded((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->configurationAdded((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 2: _t->configurationRemoved((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 3: _t->configurationChanged((*reinterpret_cast< const QNetworkConfiguration(*)>(_a[1]))); break;
        case 4: _t->updateSnapConfiguration((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QNetworkConfiguration(*)>(_a[2]))); break;
        case 5: _t->updateConfigurations(); break;
        case 6: _t->onlineStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->showConfigurationFor((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->createSessionFor((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->createNewSession(); break;
        case 10: _t->deleteSession(); break;
        case 11: _t->performScan(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BearerMonitor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BearerMonitor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_BearerMonitor,
      qt_meta_data_BearerMonitor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BearerMonitor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BearerMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BearerMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BearerMonitor))
        return static_cast<void*>(const_cast< BearerMonitor*>(this));
    if (!strcmp(_clname, "Ui_BearerMonitor"))
        return static_cast< Ui_BearerMonitor*>(const_cast< BearerMonitor*>(this));
    return QWidget::qt_metacast(_clname);
}

int BearerMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
