/****************************************************************************
** Meta object code from reading C++ file 'diagramscene.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../diagramscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'diagramscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DiagramScene[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x05,
      46,   14,   13,   13, 0x05,
      79,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,  108,   13,   13, 0x0a,
     132,  127,   13,   13, 0x0a,
     170,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DiagramScene[] = {
    "DiagramScene\0\0item\0itemInserted(DiagramItem*)\0"
    "textInserted(QGraphicsTextItem*)\0"
    "itemSelected(QGraphicsItem*)\0mode\0"
    "setMode(Mode)\0type\0"
    "setItemType(DiagramItem::DiagramType)\0"
    "editorLostFocus(DiagramTextItem*)\0"
};

void DiagramScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DiagramScene *_t = static_cast<DiagramScene *>(_o);
        switch (_id) {
        case 0: _t->itemInserted((*reinterpret_cast< DiagramItem*(*)>(_a[1]))); break;
        case 1: _t->textInserted((*reinterpret_cast< QGraphicsTextItem*(*)>(_a[1]))); break;
        case 2: _t->itemSelected((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 3: _t->setMode((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        case 4: _t->setItemType((*reinterpret_cast< DiagramItem::DiagramType(*)>(_a[1]))); break;
        case 5: _t->editorLostFocus((*reinterpret_cast< DiagramTextItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DiagramScene::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DiagramScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_DiagramScene,
      qt_meta_data_DiagramScene, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DiagramScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DiagramScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DiagramScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DiagramScene))
        return static_cast<void*>(const_cast< DiagramScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int DiagramScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
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
void DiagramScene::itemInserted(DiagramItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DiagramScene::textInserted(QGraphicsTextItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DiagramScene::itemSelected(QGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
