/****************************************************************************
** Meta object code from reading C++ file 'history.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../history.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'history.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HistoryManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       1,   54, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      36,   31,   15,   15, 0x05,
      60,   31,   15,   15, 0x05,
      93,   86,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     111,   15,   15,   15, 0x0a,
     119,   15,   15,   15, 0x0a,
     134,   15,   15,   15, 0x08,
     141,   15,   15,   15, 0x08,

 // properties: name, type, flags
     163,  159, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_HistoryManager[] = {
    "HistoryManager\0\0historyReset()\0item\0"
    "entryAdded(HistoryItem)\0"
    "entryRemoved(HistoryItem)\0offset\0"
    "entryUpdated(int)\0clear()\0loadSettings()\0"
    "save()\0checkForExpired()\0int\0historyLimit\0"
};

void HistoryManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryManager *_t = static_cast<HistoryManager *>(_o);
        switch (_id) {
        case 0: _t->historyReset(); break;
        case 1: _t->entryAdded((*reinterpret_cast< const HistoryItem(*)>(_a[1]))); break;
        case 2: _t->entryRemoved((*reinterpret_cast< const HistoryItem(*)>(_a[1]))); break;
        case 3: _t->entryUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->clear(); break;
        case 5: _t->loadSettings(); break;
        case 6: _t->save(); break;
        case 7: _t->checkForExpired(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryManager::staticMetaObject = {
    { &QWebHistoryInterface::staticMetaObject, qt_meta_stringdata_HistoryManager,
      qt_meta_data_HistoryManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryManager))
        return static_cast<void*>(const_cast< HistoryManager*>(this));
    return QWebHistoryInterface::qt_metacast(_clname);
}

int HistoryManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebHistoryInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = historyLimit(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setHistoryLimit(*reinterpret_cast< int*>(_v)); break;
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

// SIGNAL 0
void HistoryManager::historyReset()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void HistoryManager::entryAdded(const HistoryItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HistoryManager::entryRemoved(const HistoryItem & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HistoryManager::entryUpdated(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
static const uint qt_meta_data_HistoryModel[] = {

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
      14,   13,   13,   13, 0x0a,
      29,   13,   13,   13, 0x0a,
      49,   42,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HistoryModel[] = {
    "HistoryModel\0\0historyReset()\0entryAdded()\0"
    "offset\0entryUpdated(int)\0"
};

void HistoryModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryModel *_t = static_cast<HistoryModel *>(_o);
        switch (_id) {
        case 0: _t->historyReset(); break;
        case 1: _t->entryAdded(); break;
        case 2: _t->entryUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_HistoryModel,
      qt_meta_data_HistoryModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryModel))
        return static_cast<void*>(const_cast< HistoryModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int HistoryModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_HistoryFilterModel[] = {

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
      20,   19,   19,   19, 0x08,
      54,   34,   19,   19, 0x08,
     114,   97,   19,   19, 0x08,
     157,  154,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HistoryFilterModel[] = {
    "HistoryFilterModel\0\0sourceReset()\0"
    "topLeft,bottomRight\0"
    "sourceDataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "sourceRowsInserted(QModelIndex,int,int)\0"
    ",,\0sourceRowsRemoved(QModelIndex,int,int)\0"
};

void HistoryFilterModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryFilterModel *_t = static_cast<HistoryFilterModel *>(_o);
        switch (_id) {
        case 0: _t->sourceReset(); break;
        case 1: _t->sourceDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: _t->sourceRowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->sourceRowsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryFilterModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryFilterModel::staticMetaObject = {
    { &QAbstractProxyModel::staticMetaObject, qt_meta_stringdata_HistoryFilterModel,
      qt_meta_data_HistoryFilterModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryFilterModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryFilterModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryFilterModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryFilterModel))
        return static_cast<void*>(const_cast< HistoryFilterModel*>(this));
    return QAbstractProxyModel::qt_metacast(_clname);
}

int HistoryFilterModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_HistoryMenuModel[] = {

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

static const char qt_meta_stringdata_HistoryMenuModel[] = {
    "HistoryMenuModel\0"
};

void HistoryMenuModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HistoryMenuModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryMenuModel::staticMetaObject = {
    { &QAbstractProxyModel::staticMetaObject, qt_meta_stringdata_HistoryMenuModel,
      qt_meta_data_HistoryMenuModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryMenuModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryMenuModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryMenuModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryMenuModel))
        return static_cast<void*>(const_cast< HistoryMenuModel*>(this));
    return QAbstractProxyModel::qt_metacast(_clname);
}

int HistoryMenuModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_HistoryMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   31,   12,   12, 0x08,
      60,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HistoryMenu[] = {
    "HistoryMenu\0\0url\0openUrl(QUrl)\0index\0"
    "activated(QModelIndex)\0showHistoryDialog()\0"
};

void HistoryMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryMenu *_t = static_cast<HistoryMenu *>(_o);
        switch (_id) {
        case 0: _t->openUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: _t->activated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->showHistoryDialog(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryMenu::staticMetaObject = {
    { &ModelMenu::staticMetaObject, qt_meta_stringdata_HistoryMenu,
      qt_meta_data_HistoryMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryMenu))
        return static_cast<void*>(const_cast< HistoryMenu*>(this));
    return ModelMenu::qt_metacast(_clname);
}

int HistoryMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ModelMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void HistoryMenu::openUrl(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_HistoryCompletionModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HistoryCompletionModel[] = {
    "HistoryCompletionModel\0\0sourceReset()\0"
};

void HistoryCompletionModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryCompletionModel *_t = static_cast<HistoryCompletionModel *>(_o);
        switch (_id) {
        case 0: _t->sourceReset(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData HistoryCompletionModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryCompletionModel::staticMetaObject = {
    { &QAbstractProxyModel::staticMetaObject, qt_meta_stringdata_HistoryCompletionModel,
      qt_meta_data_HistoryCompletionModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryCompletionModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryCompletionModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryCompletionModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryCompletionModel))
        return static_cast<void*>(const_cast< HistoryCompletionModel*>(this));
    return QAbstractProxyModel::qt_metacast(_clname);
}

int HistoryCompletionModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_HistoryTreeModel[] = {

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
      18,   17,   17,   17, 0x08,
      49,   32,   17,   17, 0x08,
      89,   32,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HistoryTreeModel[] = {
    "HistoryTreeModel\0\0sourceReset()\0"
    "parent,start,end\0"
    "sourceRowsInserted(QModelIndex,int,int)\0"
    "sourceRowsRemoved(QModelIndex,int,int)\0"
};

void HistoryTreeModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryTreeModel *_t = static_cast<HistoryTreeModel *>(_o);
        switch (_id) {
        case 0: _t->sourceReset(); break;
        case 1: _t->sourceRowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->sourceRowsRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryTreeModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryTreeModel::staticMetaObject = {
    { &QAbstractProxyModel::staticMetaObject, qt_meta_stringdata_HistoryTreeModel,
      qt_meta_data_HistoryTreeModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryTreeModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryTreeModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryTreeModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryTreeModel))
        return static_cast<void*>(const_cast< HistoryTreeModel*>(this));
    return QAbstractProxyModel::qt_metacast(_clname);
}

int HistoryTreeModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_TreeProxyModel[] = {

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

static const char qt_meta_stringdata_TreeProxyModel[] = {
    "TreeProxyModel\0"
};

void TreeProxyModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TreeProxyModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TreeProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_TreeProxyModel,
      qt_meta_data_TreeProxyModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TreeProxyModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TreeProxyModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TreeProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TreeProxyModel))
        return static_cast<void*>(const_cast< TreeProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int TreeProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_HistoryDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   33,   14,   14, 0x08,
      72,   14,   14,   14, 0x08,
      79,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HistoryDialog[] = {
    "HistoryDialog\0\0url\0openUrl(QUrl)\0pos\0"
    "customContextMenuRequested(QPoint)\0"
    "open()\0copy()\0"
};

void HistoryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HistoryDialog *_t = static_cast<HistoryDialog *>(_o);
        switch (_id) {
        case 0: _t->openUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 1: _t->customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->open(); break;
        case 3: _t->copy(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HistoryDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HistoryDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_HistoryDialog,
      qt_meta_data_HistoryDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HistoryDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HistoryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HistoryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryDialog))
        return static_cast<void*>(const_cast< HistoryDialog*>(this));
    if (!strcmp(_clname, "Ui_HistoryDialog"))
        return static_cast< Ui_HistoryDialog*>(const_cast< HistoryDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int HistoryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void HistoryDialog::openUrl(const QUrl & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
