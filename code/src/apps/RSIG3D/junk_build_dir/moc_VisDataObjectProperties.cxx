/****************************************************************************
** Meta object code from reading C++ file 'VisDataObjectProperties.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../VisDataObjectProperties.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisDataObjectProperties.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VisDataObjectProperties[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      29,   24, 0x01195103,
      46,   40, ((uint)QMetaType::QReal << 24) | 0x00195003,
      54,   24, 0x01195103,
      71,   40, ((uint)QMetaType::QReal << 24) | 0x00195103,
      81,   40, ((uint)QMetaType::QReal << 24) | 0x00195103,

       0        // eod
};

static const char qt_meta_stringdata_VisDataObjectProperties[] = {
    "VisDataObjectProperties\0bool\0showObject\0"
    "qreal\0opacity\0showCellBoundary\0threshold\0"
    "thresholdSliderValue\0"
};

void VisDataObjectProperties::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData VisDataObjectProperties::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VisDataObjectProperties::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VisDataObjectProperties,
      qt_meta_data_VisDataObjectProperties, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VisDataObjectProperties::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VisDataObjectProperties::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VisDataObjectProperties::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VisDataObjectProperties))
        return static_cast<void*>(const_cast< VisDataObjectProperties*>(this));
    return QObject::qt_metacast(_clname);
}

int VisDataObjectProperties::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = showObject(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = alpha(); break;
        case 2: *reinterpret_cast< bool*>(_v) = showCellBoundary(); break;
        case 3: *reinterpret_cast< qreal*>(_v) = threshold(); break;
        case 4: *reinterpret_cast< qreal*>(_v) = thresholdSliderValue(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setShowObject(*reinterpret_cast< bool*>(_v)); break;
        case 1: setAlpha(*reinterpret_cast< qreal*>(_v)); break;
        case 2: setShowCellBoundary(*reinterpret_cast< bool*>(_v)); break;
        case 3: setThreshold(*reinterpret_cast< qreal*>(_v)); break;
        case 4: setThresholdSliderValue(*reinterpret_cast< qreal*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
