/****************************************************************************
** Meta object code from reading C++ file 'qdesigner_menu_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shared/qdesigner_menu_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdesigner_menu_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QDesignerMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      34,   14,   14,   14, 0x08,
      61,   14,   14,   14, 0x08,
      82,   14,   14,   14, 0x08,
     102,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QDesignerMenu[] = {
    "QDesignerMenu\0\0slotAddSeparator()\0"
    "slotRemoveSelectedAction()\0"
    "slotShowSubMenuNow()\0slotDeactivateNow()\0"
    "slotAdjustSizeNow()\0"
};

void QDesignerMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QDesignerMenu *_t = static_cast<QDesignerMenu *>(_o);
        switch (_id) {
        case 0: _t->slotAddSeparator(); break;
        case 1: _t->slotRemoveSelectedAction(); break;
        case 2: _t->slotShowSubMenuNow(); break;
        case 3: _t->slotDeactivateNow(); break;
        case 4: _t->slotAdjustSizeNow(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QDesignerMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QDesignerMenu::staticMetaObject = {
    { &QMenu::staticMetaObject, qt_meta_stringdata_QDesignerMenu,
      qt_meta_data_QDesignerMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QDesignerMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QDesignerMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QDesignerMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QDesignerMenu))
        return static_cast<void*>(const_cast< QDesignerMenu*>(this));
    return QMenu::qt_metacast(_clname);
}

int QDesignerMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
