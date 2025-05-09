/****************************************************************************
** Meta object code from reading C++ file 'canvas.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../canvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FigureEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_FigureEditor[] = {
    "FigureEditor\0\0status(QString)\0"
};

void FigureEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FigureEditor *_t = static_cast<FigureEditor *>(_o);
        switch (_id) {
        case 0: _t->status((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FigureEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FigureEditor::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_FigureEditor,
      qt_meta_data_FigureEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FigureEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FigureEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FigureEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FigureEditor))
        return static_cast<void*>(const_cast< FigureEditor*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int FigureEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FigureEditor::status(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_Main[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      13,    5,    5,    5, 0x08,
      23,    5,    5,    5, 0x08,
      33,    5,    5,    5, 0x08,
      41,    5,    5,    5, 0x08,
      48,    5,    5,    5, 0x08,
      60,    5,    5,    5, 0x08,
      72,    5,    5,    5, 0x08,
      85,    5,    5,    5, 0x08,
      98,    5,    5,    5, 0x08,
     110,    5,    5,    5, 0x08,
     120,    5,    5,    5, 0x08,
     130,    5,    5,    5, 0x08,
     145,    5,    5,    5, 0x08,
     155,    5,    5,    5, 0x08,
     165,    5,    5,    5, 0x08,
     180,    5,    5,    5, 0x08,
     190,    5,    5,    5, 0x08,
     199,    5,    5,    5, 0x08,
     217,    5,    5,    5, 0x08,
     242,    5,    5,    5, 0x08,
     251,    5,    5,    5, 0x08,
     261,    5,    5,    5, 0x08,
     270,    5,    5,    5, 0x08,
     278,    5,    5,    5, 0x08,
     286,    5,    5,    5, 0x08,
     294,    5,    5,    5, 0x08,
     302,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Main[] = {
    "Main\0\0help()\0aboutQt()\0newView()\0"
    "clear()\0init()\0addSprite()\0addCircle()\0"
    "addHexagon()\0addPolygon()\0addSpline()\0"
    "addText()\0addLine()\0addRectangle()\0"
    "addMesh()\0addLogo()\0addButterfly()\0"
    "enlarge()\0shrink()\0rotateClockwise()\0"
    "rotateCounterClockwise()\0zoomIn()\0"
    "zoomOut()\0mirror()\0moveL()\0moveR()\0"
    "moveU()\0moveD()\0print()\0"
};

void Main::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Main *_t = static_cast<Main *>(_o);
        switch (_id) {
        case 0: _t->help(); break;
        case 1: _t->aboutQt(); break;
        case 2: _t->newView(); break;
        case 3: _t->clear(); break;
        case 4: _t->init(); break;
        case 5: _t->addSprite(); break;
        case 6: _t->addCircle(); break;
        case 7: _t->addHexagon(); break;
        case 8: _t->addPolygon(); break;
        case 9: _t->addSpline(); break;
        case 10: _t->addText(); break;
        case 11: _t->addLine(); break;
        case 12: _t->addRectangle(); break;
        case 13: _t->addMesh(); break;
        case 14: _t->addLogo(); break;
        case 15: _t->addButterfly(); break;
        case 16: _t->enlarge(); break;
        case 17: _t->shrink(); break;
        case 18: _t->rotateClockwise(); break;
        case 19: _t->rotateCounterClockwise(); break;
        case 20: _t->zoomIn(); break;
        case 21: _t->zoomOut(); break;
        case 22: _t->mirror(); break;
        case 23: _t->moveL(); break;
        case 24: _t->moveR(); break;
        case 25: _t->moveU(); break;
        case 26: _t->moveD(); break;
        case 27: _t->print(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Main::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Main::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Main,
      qt_meta_data_Main, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Main::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Main::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Main::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Main))
        return static_cast<void*>(const_cast< Main*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Main::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
