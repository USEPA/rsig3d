/****************************************************************************
** Meta object code from reading C++ file 'VisDataPropertyEdit.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VisDataPropertyEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisDataPropertyEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VisDataPropertyEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      53,   20,   20,   20, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_VisDataPropertyEdit[] = {
    "VisDataPropertyEdit\0\0"
    "onParentObjectRemoved(QObject*)\0"
    "showContextMenu(QPoint)\0"
};

void VisDataPropertyEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        VisDataPropertyEdit *_t = static_cast<VisDataPropertyEdit *>(_o);
        switch (_id) {
        case 0: _t->onParentObjectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 1: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData VisDataPropertyEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VisDataPropertyEdit::staticMetaObject = {
    { &PropertyEditor::PropertyEdit::staticMetaObject, qt_meta_stringdata_VisDataPropertyEdit,
      qt_meta_data_VisDataPropertyEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VisDataPropertyEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VisDataPropertyEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VisDataPropertyEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VisDataPropertyEdit))
        return static_cast<void*>(const_cast< VisDataPropertyEdit*>(this));
    typedef PropertyEditor::PropertyEdit QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int VisDataPropertyEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef PropertyEditor::PropertyEdit QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
