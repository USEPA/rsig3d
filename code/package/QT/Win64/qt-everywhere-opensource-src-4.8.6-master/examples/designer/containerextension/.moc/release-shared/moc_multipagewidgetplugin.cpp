/****************************************************************************
** Meta object code from reading C++ file 'multipagewidgetplugin.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../multipagewidgetplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multipagewidgetplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MultiPageWidgetPlugin[] = {

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
      29,   23,   22,   22, 0x08,
      60,   54,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MultiPageWidgetPlugin[] = {
    "MultiPageWidgetPlugin\0\0index\0"
    "currentIndexChanged(int)\0title\0"
    "pageTitleChanged(QString)\0"
};

void MultiPageWidgetPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MultiPageWidgetPlugin *_t = static_cast<MultiPageWidgetPlugin *>(_o);
        switch (_id) {
        case 0: _t->currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->pageTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MultiPageWidgetPlugin::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MultiPageWidgetPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MultiPageWidgetPlugin,
      qt_meta_data_MultiPageWidgetPlugin, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MultiPageWidgetPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MultiPageWidgetPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MultiPageWidgetPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MultiPageWidgetPlugin))
        return static_cast<void*>(const_cast< MultiPageWidgetPlugin*>(this));
    if (!strcmp(_clname, "QDesignerCustomWidgetInterface"))
        return static_cast< QDesignerCustomWidgetInterface*>(const_cast< MultiPageWidgetPlugin*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.Designer.CustomWidget"))
        return static_cast< QDesignerCustomWidgetInterface*>(const_cast< MultiPageWidgetPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int MultiPageWidgetPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
