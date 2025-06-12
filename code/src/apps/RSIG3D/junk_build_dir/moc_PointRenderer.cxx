/****************************************************************************
** Meta object code from reading C++ file 'PointRenderer.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PointRenderer.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PointRenderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PointRenderer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      18,   14, 0x02195103,
      35,   29, ((uint)QMetaType::QReal << 24) | 0x00195103,
      45,   29, ((uint)QMetaType::QReal << 24) | 0x00195103,
      62,   55, 0x43195103,
      78,   73, 0x01195103,
      93,   73, 0x01195103,

       0        // eod
};

static const char qt_meta_stringdata_PointRenderer[] = {
    "PointRenderer\0int\0pointShape\0qreal\0"
    "pointSize\0lineWidth\0QColor\0pointColor\0"
    "bool\0showNullSample\0useOpenglPointSmooth\0"
};

void PointRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PointRenderer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PointRenderer::staticMetaObject = {
    { &VisDataObject::staticMetaObject, qt_meta_stringdata_PointRenderer,
      qt_meta_data_PointRenderer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PointRenderer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PointRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PointRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PointRenderer))
        return static_cast<void*>(const_cast< PointRenderer*>(this));
    return VisDataObject::qt_metacast(_clname);
}

int PointRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VisDataObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = pointShape(); break;
        case 1: *reinterpret_cast< qreal*>(_v) = pointSize(); break;
        case 2: *reinterpret_cast< qreal*>(_v) = lineWidth(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = pointColor(); break;
        case 4: *reinterpret_cast< bool*>(_v) = showNullSample(); break;
        case 5: *reinterpret_cast< bool*>(_v) = useOpenglPointSmooth(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPointShape(*reinterpret_cast< int*>(_v)); break;
        case 1: setPointSize(*reinterpret_cast< qreal*>(_v)); break;
        case 2: setLineWidth(*reinterpret_cast< qreal*>(_v)); break;
        case 3: setPointColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setShowNullSample(*reinterpret_cast< bool*>(_v)); break;
        case 5: setUseOpenglPointSmooth(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
