/****************************************************************************
** Meta object code from reading C++ file 'paletteeditorbutton.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../propertyeditor/paletteeditorbutton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paletteeditorbutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qdesigner_internal__PaletteEditorButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      49,   41,   40,   40, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   41,   40,   40, 0x0a,
      95,   40,   40,   40, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qdesigner_internal__PaletteEditorButton[] = {
    "qdesigner_internal::PaletteEditorButton\0"
    "\0palette\0paletteChanged(QPalette)\0"
    "setPalette(QPalette)\0showPaletteEditor()\0"
};

void qdesigner_internal::PaletteEditorButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PaletteEditorButton *_t = static_cast<PaletteEditorButton *>(_o);
        switch (_id) {
        case 0: _t->paletteChanged((*reinterpret_cast< const QPalette(*)>(_a[1]))); break;
        case 1: _t->setPalette((*reinterpret_cast< const QPalette(*)>(_a[1]))); break;
        case 2: _t->showPaletteEditor(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData qdesigner_internal::PaletteEditorButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject qdesigner_internal::PaletteEditorButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_qdesigner_internal__PaletteEditorButton,
      qt_meta_data_qdesigner_internal__PaletteEditorButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qdesigner_internal::PaletteEditorButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qdesigner_internal::PaletteEditorButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qdesigner_internal::PaletteEditorButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qdesigner_internal__PaletteEditorButton))
        return static_cast<void*>(const_cast< PaletteEditorButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int qdesigner_internal::PaletteEditorButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void qdesigner_internal::PaletteEditorButton::paletteChanged(const QPalette & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
