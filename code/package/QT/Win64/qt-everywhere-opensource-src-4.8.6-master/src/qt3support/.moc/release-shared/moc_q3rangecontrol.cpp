/****************************************************************************
** Meta object code from reading C++ file 'q3rangecontrol.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widgets/q3rangecontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'q3rangecontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Q3SpinWidget[] = {

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
      14,   13,   13,   13, 0x05,
      30,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   13,   13,   13, 0x0a,
      57,   13,   13,   13, 0x0a,
      68,   13,   13,   13, 0x08,
      80,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Q3SpinWidget[] = {
    "Q3SpinWidget\0\0stepUpPressed()\0"
    "stepDownPressed()\0stepUp()\0stepDown()\0"
    "timerDone()\0timerDoneEx()\0"
};

void Q3SpinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q3SpinWidget *_t = static_cast<Q3SpinWidget *>(_o);
        switch (_id) {
        case 0: _t->stepUpPressed(); break;
        case 1: _t->stepDownPressed(); break;
        case 2: _t->stepUp(); break;
        case 3: _t->stepDown(); break;
        case 4: _t->timerDone(); break;
        case 5: _t->timerDoneEx(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Q3SpinWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q3SpinWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Q3SpinWidget,
      qt_meta_data_Q3SpinWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q3SpinWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q3SpinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q3SpinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q3SpinWidget))
        return static_cast<void*>(const_cast< Q3SpinWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Q3SpinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Q3SpinWidget::stepUpPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Q3SpinWidget::stepDownPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
