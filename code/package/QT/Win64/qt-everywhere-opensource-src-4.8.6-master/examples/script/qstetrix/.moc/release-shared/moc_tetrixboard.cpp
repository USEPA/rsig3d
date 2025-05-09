/****************************************************************************
** Meta object code from reading C++ file 'tetrixboard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tetrixboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tetrixboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TetrixBoard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       2,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,
      43,   37,   12,   12, 0x05,
      70,   61,   12,   12, 0x05,
      99,   95,   12,   12, 0x05,
     123,  115,   12,   12, 0x05,
     149,  115,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     197,  184,   12,   12, 0x09,
     220,  115,   12,   12, 0x09,
     265,  247,   12,   12, 0x09,
     299,   12,   12,   12, 0x09,

 // properties: name, type, flags
     317,  308, 0x88095001,
     332,  323, 0x89095103,

       0        // eod
};

static const char qt_meta_stringdata_TetrixBoard[] = {
    "TetrixBoard\0\0score\0scoreChanged(int)\0"
    "level\0levelChanged(int)\0numLines\0"
    "linesRemovedChanged(int)\0key\0"
    "keyPressed(int)\0painter\0"
    "paintRequested(QPainter*)\0"
    "paintNextPieceRequested(QPainter*)\0"
    "width,height\0showNextPiece(int,int)\0"
    "drawPauseScreen(QPainter*)\0painter,x,y,shape\0"
    "drawSquare(QPainter*,int,int,int)\0"
    "update()\0QObject*\0timer\0QWidget*\0"
    "nextPieceLabel\0"
};

void TetrixBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TetrixBoard *_t = static_cast<TetrixBoard *>(_o);
        switch (_id) {
        case 0: _t->scoreChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->levelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->linesRemovedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->keyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->paintRequested((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 5: _t->paintNextPieceRequested((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 6: _t->showNextPiece((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->drawPauseScreen((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 8: _t->drawSquare((*reinterpret_cast< QPainter*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 9: _t->update(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TetrixBoard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TetrixBoard::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_TetrixBoard,
      qt_meta_data_TetrixBoard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TetrixBoard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TetrixBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TetrixBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TetrixBoard))
        return static_cast<void*>(const_cast< TetrixBoard*>(this));
    return QFrame::qt_metacast(_clname);
}

int TetrixBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QObject**>(_v) = getTimer(); break;
        case 1: *reinterpret_cast< QWidget**>(_v) = nextPieceLabel(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setNextPieceLabel(*reinterpret_cast< QWidget**>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void TetrixBoard::scoreChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TetrixBoard::levelChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TetrixBoard::linesRemovedChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TetrixBoard::keyPressed(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TetrixBoard::paintRequested(QPainter * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TetrixBoard::paintNextPieceRequested(QPainter * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
