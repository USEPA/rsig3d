/****************************************************************************
** Meta object code from reading C++ file 'spreadsheet.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../spreadsheet.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spreadsheet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpreadSheet[] = {

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
      18,   13,   12,   12, 0x0a,
      50,   13,   12,   12, 0x0a,
      81,   13,   12,   12, 0x0a,
     115,   12,   12,   12, 0x0a,
     131,   12,   12,   12, 0x0a,
     145,   12,   12,   12, 0x0a,
     158,   12,   12,   12, 0x0a,
     166,   12,   12,   12, 0x0a,
     178,   12,   12,   12, 0x0a,
     186,   12,   12,   12, 0x0a,
     198,   12,   12,   12, 0x0a,
     215,   12,   12,   12, 0x0a,
     227,   12,   12,   12, 0x0a,
     244,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SpreadSheet[] = {
    "SpreadSheet\0\0item\0updateStatus(QTableWidgetItem*)\0"
    "updateColor(QTableWidgetItem*)\0"
    "updateLineEdit(QTableWidgetItem*)\0"
    "returnPressed()\0selectColor()\0"
    "selectFont()\0clear()\0showAbout()\0"
    "print()\0actionSum()\0actionSubtract()\0"
    "actionAdd()\0actionMultiply()\0"
    "actionDivide()\0"
};

void SpreadSheet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SpreadSheet *_t = static_cast<SpreadSheet *>(_o);
        switch (_id) {
        case 0: _t->updateStatus((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->updateColor((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->updateLineEdit((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->returnPressed(); break;
        case 4: _t->selectColor(); break;
        case 5: _t->selectFont(); break;
        case 6: _t->clear(); break;
        case 7: _t->showAbout(); break;
        case 8: _t->print(); break;
        case 9: _t->actionSum(); break;
        case 10: _t->actionSubtract(); break;
        case 11: _t->actionAdd(); break;
        case 12: _t->actionMultiply(); break;
        case 13: _t->actionDivide(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SpreadSheet::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SpreadSheet::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SpreadSheet,
      qt_meta_data_SpreadSheet, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SpreadSheet::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SpreadSheet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SpreadSheet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpreadSheet))
        return static_cast<void*>(const_cast< SpreadSheet*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SpreadSheet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
