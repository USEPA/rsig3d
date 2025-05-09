/****************************************************************************
** Meta object code from reading C++ file 'view.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../view.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KAsteroidsView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      34,   29,   15,   15, 0x05,
      47,   15,   15,   15, 0x05,
      62,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KAsteroidsView[] = {
    "KAsteroidsView\0\0shipKilled()\0size\0"
    "rockHit(int)\0rocksRemoved()\0updateVitals()\0"
    "hideShield()\0"
};

void KAsteroidsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KAsteroidsView *_t = static_cast<KAsteroidsView *>(_o);
        switch (_id) {
        case 0: _t->shipKilled(); break;
        case 1: _t->rockHit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->rocksRemoved(); break;
        case 3: _t->updateVitals(); break;
        case 4: _t->hideShield(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData KAsteroidsView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject KAsteroidsView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_KAsteroidsView,
      qt_meta_data_KAsteroidsView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KAsteroidsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KAsteroidsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KAsteroidsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KAsteroidsView))
        return static_cast<void*>(const_cast< KAsteroidsView*>(this));
    return QWidget::qt_metacast(_clname);
}

int KAsteroidsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KAsteroidsView::shipKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void KAsteroidsView::rockHit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void KAsteroidsView::rocksRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void KAsteroidsView::updateVitals()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
