/****************************************************************************
** Meta object code from reading C++ file 'newdynamicpropertydialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../propertyeditor/newdynamicpropertydialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newdynamicpropertydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdesigner_internal__NewDynamicPropertyDialog[] = {

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
      50,   46,   45,   45, 0x08,
      91,   45,   45,   45, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdesigner_internal__NewDynamicPropertyDialog[] = {
    "qdesigner_internal::NewDynamicPropertyDialog\0"
    "\0btn\0on_m_buttonBox_clicked(QAbstractButton*)\0"
    "nameChanged(QString)\0"
};

void qdesigner_internal::NewDynamicPropertyDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NewDynamicPropertyDialog *_t = static_cast<NewDynamicPropertyDialog *>(_o);
        switch (_id) {
        case 0: _t->on_m_buttonBox_clicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 1: _t->nameChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qdesigner_internal::NewDynamicPropertyDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qdesigner_internal::NewDynamicPropertyDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_qdesigner_internal__NewDynamicPropertyDialog,
      qt_meta_data_qdesigner_internal__NewDynamicPropertyDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qdesigner_internal::NewDynamicPropertyDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qdesigner_internal::NewDynamicPropertyDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qdesigner_internal::NewDynamicPropertyDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdesigner_internal__NewDynamicPropertyDialog))
        return static_cast<void*>(const_cast< NewDynamicPropertyDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int qdesigner_internal::NewDynamicPropertyDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
