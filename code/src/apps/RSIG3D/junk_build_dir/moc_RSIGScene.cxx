/****************************************************************************
** Meta object code from reading C++ file 'RSIGScene.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../RSIGScene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RSIGScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RSIGScene[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   10,   10,   10, 0x0a,
      73,   67,   10,   10, 0x0a,
     104,   99,   10,   10, 0x0a,
     132,  126,   10,   10, 0x0a,
     158,   10,   10,   10, 0x0a,
     169,   10,   10,   10, 0x0a,
     181,   10,   10,   10, 0x0a,
     190,   10,   10,   10, 0x0a,
     201,   10,   10,   10, 0x0a,
     210,   10,   10,   10, 0x0a,
     220,   10,   10,   10, 0x0a,
     237,   10,   10,   10, 0x0a,
     246,   10,   10,   10, 0x0a,
     259,   10,   10,   10, 0x0a,
     271,   10,   10,   10, 0x0a,
     289,   10,   10,   10, 0x0a,
     306,   10,   10,   10, 0x0a,
     338,  324,   10,   10, 0x0a,
     394,  385,  378,   10, 0x0a,
     436,  424,  416,   10, 0x0a,
     457,   10,  378,   10, 0x0a,
     474,   11,   10,   10, 0x0a,
     505,   10,  500,   10, 0x0a,
     523,   10,  500,   10, 0x0a,
     553,   10,  500,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RSIGScene[] = {
    "RSIGScene\0\0rect\0boundingGeoBoxChanged(QRectF)\0"
    "setBackgroundColor()\0theme\0"
    "setMapServer(const char*)\0mode\0"
    "setMapProjection(int)\0rectf\0"
    "setBoundingGeoBox(QRectF)\0moveLeft()\0"
    "moveRight()\0moveUp()\0moveDown()\0"
    "zoomIn()\0zoomOut()\0zoomChanged(int)\0"
    "goHome()\0goAutozoom()\0goLambert()\0"
    "goStereographic()\0centerAt(QRectF)\0"
    "centerAt(QPointF)\0dx,dy,sdx,sdy\0"
    "centerByOffset(qreal,qreal,qreal,qreal)\0"
    "QRectF\0geoRectF\0toScreenRectF(QRectF)\0"
    "QPointF\0screenCoord\0toGeoPointF(QPointF)\0"
    "screenGeoRectF()\0setScreenGeoRectF(QRectF)\0"
    "bool\0isSphericalProj()\0"
    "isLambertConformalConicProj()\0"
    "isStereographicProj()\0"
};

void RSIGScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RSIGScene *_t = static_cast<RSIGScene *>(_o);
        switch (_id) {
        case 0: _t->boundingGeoBoxChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 1: _t->setBackgroundColor(); break;
        case 2: _t->setMapServer((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 3: _t->setMapProjection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setBoundingGeoBox((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 5: _t->moveLeft(); break;
        case 6: _t->moveRight(); break;
        case 7: _t->moveUp(); break;
        case 8: _t->moveDown(); break;
        case 9: _t->zoomIn(); break;
        case 10: _t->zoomOut(); break;
        case 11: _t->zoomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->goHome(); break;
        case 13: _t->goAutozoom(); break;
        case 14: _t->goLambert(); break;
        case 15: _t->goStereographic(); break;
        case 16: _t->centerAt((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 17: _t->centerAt((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 18: _t->centerByOffset((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< qreal(*)>(_a[4]))); break;
        case 19: { QRectF _r = _t->toScreenRectF((*reinterpret_cast< const QRectF(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = _r; }  break;
        case 20: { QPointF _r = _t->toGeoPointF((*reinterpret_cast< const QPointF(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QPointF*>(_a[0]) = _r; }  break;
        case 21: { QRectF _r = _t->screenGeoRectF();
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = _r; }  break;
        case 22: _t->setScreenGeoRectF((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 23: { bool _r = _t->isSphericalProj();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 24: { bool _r = _t->isLambertConformalConicProj();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 25: { bool _r = _t->isStereographicProj();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RSIGScene::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RSIGScene::staticMetaObject = {
    { &OpenGLScene::staticMetaObject, qt_meta_stringdata_RSIGScene,
      qt_meta_data_RSIGScene, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RSIGScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RSIGScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RSIGScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RSIGScene))
        return static_cast<void*>(const_cast< RSIGScene*>(this));
    return OpenGLScene::qt_metacast(_clname);
}

int RSIGScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OpenGLScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void RSIGScene::boundingGeoBoxChanged(const QRectF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
