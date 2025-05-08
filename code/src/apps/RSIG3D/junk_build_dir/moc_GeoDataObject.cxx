/****************************************************************************
** Meta object code from reading C++ file 'GeoDataObject.h'
**
** Created: Thu May 8 12:58:26 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GeoDataObject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GeoDataObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GeoDataObject[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
      21,   39, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x05,
      50,   15,   14,   14, 0x05,
      80,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     110,   14,   95,   14, 0x0a,
     164,   14,  145,   14, 0x0a,

 // properties: name, type, flags
     196,  191, 0x01195103,
     215,  207, 0x0a195401,
     223,  207, 0x0a195401,
     242,  235, 0x14195401,
     242,  235, 0x14195401,
     249,  207, 0x0a195401,
     259,  207, 0x0a195001,
     271,  267, 0x02195401,
     281,  267, 0x02195401,
     290,  191, 0x01195401,
     303,  191, 0x01195401,
     311,  191, 0x01195401,
     321,  191, 0x01195401,
     334,  191, 0x01195401,
     342,  191, 0x01195401,
     366,  359, 0x06195401,
     374,  359, 0x06195401,
     382,  207, 0x0a195401,
     388,  267, 0x02195401,
     393,  267, 0x02195401,
     401,  267, 0x02195401,

       0        // eod
};

static const char qt_meta_stringdata_GeoDataObject[] = {
    "GeoDataObject\0\0obj\0objectInserted(GeoDataObject*)\0"
    "objectRemoved(GeoDataObject*)\0"
    "valueChanged()\0VisDataObject*\0"
    "createDefaultVisualizationObject()\0"
    "LookupTableWidget*\0createDefaultLookupTable()\0"
    "bool\0showObject\0QString\0varName\0"
    "description\0QRectF\0bounds\0startDate\0"
    "endDate\0int\0timesteps\0cellType\0"
    "hasElevation\0hasArea\0hasVolume\0"
    "isStationary\0isGroup\0isStructuredGrid\0"
    "double\0dataMin\0dataMax\0units\0rows\0"
    "columns\0layers\0"
};

void GeoDataObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GeoDataObject *_t = static_cast<GeoDataObject *>(_o);
        switch (_id) {
        case 0: _t->objectInserted((*reinterpret_cast< GeoDataObject*(*)>(_a[1]))); break;
        case 1: _t->objectRemoved((*reinterpret_cast< GeoDataObject*(*)>(_a[1]))); break;
        case 2: _t->valueChanged(); break;
        case 3: { VisDataObject* _r = _t->createDefaultVisualizationObject();
            if (_a[0]) *reinterpret_cast< VisDataObject**>(_a[0]) = _r; }  break;
        case 4: { LookupTableWidget* _r = _t->createDefaultLookupTable();
            if (_a[0]) *reinterpret_cast< LookupTableWidget**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GeoDataObject::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GeoDataObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GeoDataObject,
      qt_meta_data_GeoDataObject, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GeoDataObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GeoDataObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GeoDataObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeoDataObject))
        return static_cast<void*>(const_cast< GeoDataObject*>(this));
    return QObject::qt_metacast(_clname);
}

int GeoDataObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = getShowObject(); break;
        case 1: *reinterpret_cast< QString*>(_v) = varName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = description(); break;
        case 3: *reinterpret_cast< QRectF*>(_v) = getRegionRect(); break;
        case 4: *reinterpret_cast< QRectF*>(_v) = getDataRequestBoundingBox(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getStartDate(); break;
        case 6: *reinterpret_cast< QString*>(_v) = getEndDate(); break;
        case 7: *reinterpret_cast< int*>(_v) = timesteps(); break;
        case 8: *reinterpret_cast< int*>(_v) = cellType(); break;
        case 9: *reinterpret_cast< bool*>(_v) = hasElevation(); break;
        case 10: *reinterpret_cast< bool*>(_v) = hasArea(); break;
        case 11: *reinterpret_cast< bool*>(_v) = hasVolume(); break;
        case 12: *reinterpret_cast< bool*>(_v) = isStationary(); break;
        case 13: *reinterpret_cast< bool*>(_v) = isGroup(); break;
        case 14: *reinterpret_cast< bool*>(_v) = isStructuredGrid(); break;
        case 15: *reinterpret_cast< double*>(_v) = dataMin(); break;
        case 16: *reinterpret_cast< double*>(_v) = dataMax(); break;
        case 17: *reinterpret_cast< QString*>(_v) = units(); break;
        case 18: *reinterpret_cast< int*>(_v) = rows(); break;
        case 19: *reinterpret_cast< int*>(_v) = columns(); break;
        case 20: *reinterpret_cast< int*>(_v) = layers(); break;
        }
        _id -= 21;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setShowObject(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 21;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 21;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 21;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GeoDataObject::objectInserted(GeoDataObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GeoDataObject::objectRemoved(GeoDataObject * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GeoDataObject::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
