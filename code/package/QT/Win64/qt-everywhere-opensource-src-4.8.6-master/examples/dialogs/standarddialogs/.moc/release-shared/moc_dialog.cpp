/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      21,    7,    7,    7, 0x08,
      33,    7,    7,    7, 0x08,
      43,    7,    7,    7, 0x08,
      53,    7,    7,    7, 0x08,
      64,    7,    7,    7, 0x08,
      74,    7,    7,    7, 0x08,
      97,    7,    7,    7, 0x08,
     115,    7,    7,    7, 0x08,
     134,    7,    7,    7, 0x08,
     152,    7,    7,    7, 0x08,
     170,    7,    7,    7, 0x08,
     191,    7,    7,    7, 0x08,
     209,    7,    7,    7, 0x08,
     226,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Dialog[] = {
    "Dialog\0\0setInteger()\0setDouble()\0"
    "setItem()\0setText()\0setColor()\0setFont()\0"
    "setExistingDirectory()\0setOpenFileName()\0"
    "setOpenFileNames()\0setSaveFileName()\0"
    "criticalMessage()\0informationMessage()\0"
    "questionMessage()\0warningMessage()\0"
    "errorMessage()\0"
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dialog *_t = static_cast<Dialog *>(_o);
        switch (_id) {
        case 0: _t->setInteger(); break;
        case 1: _t->setDouble(); break;
        case 2: _t->setItem(); break;
        case 3: _t->setText(); break;
        case 4: _t->setColor(); break;
        case 5: _t->setFont(); break;
        case 6: _t->setExistingDirectory(); break;
        case 7: _t->setOpenFileName(); break;
        case 8: _t->setOpenFileNames(); break;
        case 9: _t->setSaveFileName(); break;
        case 10: _t->criticalMessage(); break;
        case 11: _t->informationMessage(); break;
        case 12: _t->questionMessage(); break;
        case 13: _t->warningMessage(); break;
        case 14: _t->errorMessage(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Dialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Dialog,
      qt_meta_data_Dialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
