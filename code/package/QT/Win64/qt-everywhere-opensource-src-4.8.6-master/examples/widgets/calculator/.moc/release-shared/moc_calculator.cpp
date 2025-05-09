/****************************************************************************
** Meta object code from reading C++ file 'calculator.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../calculator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calculator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Calculator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      27,   11,   11,   11, 0x08,
      50,   11,   11,   11, 0x08,
      76,   11,   11,   11, 0x08,
     108,   11,   11,   11, 0x08,
     123,   11,   11,   11, 0x08,
     138,   11,   11,   11, 0x08,
     158,   11,   11,   11, 0x08,
     177,   11,   11,   11, 0x08,
     185,   11,   11,   11, 0x08,
     196,   11,   11,   11, 0x08,
     210,   11,   11,   11, 0x08,
     223,   11,   11,   11, 0x08,
     235,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Calculator[] = {
    "Calculator\0\0digitClicked()\0"
    "unaryOperatorClicked()\0additiveOperatorClicked()\0"
    "multiplicativeOperatorClicked()\0"
    "equalClicked()\0pointClicked()\0"
    "changeSignClicked()\0backspaceClicked()\0"
    "clear()\0clearAll()\0clearMemory()\0"
    "readMemory()\0setMemory()\0addToMemory()\0"
};

void Calculator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Calculator *_t = static_cast<Calculator *>(_o);
        switch (_id) {
        case 0: _t->digitClicked(); break;
        case 1: _t->unaryOperatorClicked(); break;
        case 2: _t->additiveOperatorClicked(); break;
        case 3: _t->multiplicativeOperatorClicked(); break;
        case 4: _t->equalClicked(); break;
        case 5: _t->pointClicked(); break;
        case 6: _t->changeSignClicked(); break;
        case 7: _t->backspaceClicked(); break;
        case 8: _t->clear(); break;
        case 9: _t->clearAll(); break;
        case 10: _t->clearMemory(); break;
        case 11: _t->readMemory(); break;
        case 12: _t->setMemory(); break;
        case 13: _t->addToMemory(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Calculator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Calculator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Calculator,
      qt_meta_data_Calculator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Calculator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Calculator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Calculator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Calculator))
        return static_cast<void*>(const_cast< Calculator*>(this));
    return QWidget::qt_metacast(_clname);
}

int Calculator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
