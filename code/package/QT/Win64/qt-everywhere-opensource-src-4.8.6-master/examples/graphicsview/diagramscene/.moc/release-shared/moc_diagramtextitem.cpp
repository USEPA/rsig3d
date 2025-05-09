/****************************************************************************
** Meta object code from reading C++ file 'diagramtextitem.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../diagramtextitem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'diagramtextitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DiagramTextItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   17,   16,   16, 0x05,
      50,   17,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DiagramTextItem[] = {
    "DiagramTextItem\0\0item\0lostFocus(DiagramTextItem*)\0"
    "selectedChange(QGraphicsItem*)\0"
};

void DiagramTextItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DiagramTextItem *_t = static_cast<DiagramTextItem *>(_o);
        switch (_id) {
        case 0: _t->lostFocus((*reinterpret_cast< DiagramTextItem*(*)>(_a[1]))); break;
        case 1: _t->selectedChange((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DiagramTextItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DiagramTextItem::staticMetaObject = {
    { &QGraphicsTextItem::staticMetaObject, qt_meta_stringdata_DiagramTextItem,
      qt_meta_data_DiagramTextItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DiagramTextItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DiagramTextItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DiagramTextItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DiagramTextItem))
        return static_cast<void*>(const_cast< DiagramTextItem*>(this));
    return QGraphicsTextItem::qt_metacast(_clname);
}

int DiagramTextItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsTextItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DiagramTextItem::lostFocus(DiagramTextItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DiagramTextItem::selectedChange(QGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
