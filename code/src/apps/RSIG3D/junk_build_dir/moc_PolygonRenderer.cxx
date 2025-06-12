/****************************************************************************
** Meta object code from reading C++ file 'PolygonRenderer.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PolygonRenderer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PolygonRenderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PolygonRenderer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      22,   16, ((uint)QMetaType::QReal << 24) | 0x00195103,
      39,   32, 0x43195103,
      54,   49, 0x01195103,

       0        // eod
};

static const char qt_meta_stringdata_PolygonRenderer[] = {
    "PolygonRenderer\0qreal\0lineWidth\0QColor\0"
    "lineColor\0bool\0showNullSample\0"
};

void PolygonRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PolygonRenderer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PolygonRenderer::staticMetaObject = {
    { &VisDataObject::staticMetaObject, qt_meta_stringdata_PolygonRenderer,
      qt_meta_data_PolygonRenderer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PolygonRenderer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PolygonRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PolygonRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PolygonRenderer))
        return static_cast<void*>(const_cast< PolygonRenderer*>(this));
    return VisDataObject::qt_metacast(_clname);
}

int PolygonRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VisDataObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< qreal*>(_v) = lineWidth(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = lineColor(); break;
        case 2: *reinterpret_cast< bool*>(_v) = showNullSample(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLineWidth(*reinterpret_cast< qreal*>(_v)); break;
        case 1: setLineColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: setShowNullSample(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
