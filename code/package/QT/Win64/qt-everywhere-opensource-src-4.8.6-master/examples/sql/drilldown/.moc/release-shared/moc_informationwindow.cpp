/****************************************************************************
** Meta object code from reading C++ file 'informationwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../informationwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'informationwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_InformationWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   18,   18,   18, 0x08,
      66,   18,   18,   18, 0x08,
      82,   75,   18,   18, 0x08,
     102,   18,   18,   18, 0x28,

       0        // eod
};

static const char qt_meta_stringdata_InformationWindow[] = {
    "InformationWindow\0\0id,fileName\0"
    "imageChanged(int,QString)\0revert()\0"
    "submit()\0enable\0enableButtons(bool)\0"
    "enableButtons()\0"
};

void InformationWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        InformationWindow *_t = static_cast<InformationWindow *>(_o);
        switch (_id) {
        case 0: _t->imageChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->revert(); break;
        case 2: _t->submit(); break;
        case 3: _t->enableButtons((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->enableButtons(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData InformationWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject InformationWindow::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_InformationWindow,
      qt_meta_data_InformationWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &InformationWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *InformationWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *InformationWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InformationWindow))
        return static_cast<void*>(const_cast< InformationWindow*>(this));
    return QDialog::qt_metacast(_clname);
}

int InformationWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void InformationWindow::imageChanged(int _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
