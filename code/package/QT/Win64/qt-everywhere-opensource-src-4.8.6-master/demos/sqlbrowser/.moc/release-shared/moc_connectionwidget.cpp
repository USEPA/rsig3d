/****************************************************************************
** Meta object code from reading C++ file 'connectionwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../connectionwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConnectionWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,
      58,   48,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   17,   17,   17, 0x0a,
      95,   17,   17,   17, 0x0a,
     122,  110,   17,   17, 0x0a,
     183,  166,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ConnectionWidget[] = {
    "ConnectionWidget\0\0table\0tableActivated(QString)\0"
    "tableName\0metaDataRequested(QString)\0"
    "refresh()\0showMetaData()\0item,column\0"
    "on_tree_itemActivated(QTreeWidgetItem*,int)\0"
    "current,previous\0"
    "on_tree_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)\0"
};

void ConnectionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ConnectionWidget *_t = static_cast<ConnectionWidget *>(_o);
        switch (_id) {
        case 0: _t->tableActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->metaDataRequested((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->refresh(); break;
        case 3: _t->showMetaData(); break;
        case 4: _t->on_tree_itemActivated((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->on_tree_currentItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ConnectionWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ConnectionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConnectionWidget,
      qt_meta_data_ConnectionWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConnectionWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConnectionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConnectionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionWidget))
        return static_cast<void*>(const_cast< ConnectionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConnectionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ConnectionWidget::tableActivated(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConnectionWidget::metaDataRequested(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
