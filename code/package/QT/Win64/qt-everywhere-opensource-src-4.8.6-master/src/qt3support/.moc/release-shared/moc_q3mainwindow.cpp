/****************************************************************************
** Meta object code from reading C++ file 'q3mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widgets/q3mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'q3mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Q3MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       5,  109, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      38,   13,   13,   13, 0x05,
      65,   13,   13,   13, 0x05,
     106,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     141,   13,   13,   13, 0x0a,
     169,   13,   13,   13, 0x0a,
     193,   13,   13,   13, 0x0a,
     216,   13,   13,   13, 0x0a,
     244,   13,   13,   13, 0x0a,
     266,   13,   13,   13, 0x0a,
     291,   13,   13,   13, 0x0a,
     308,  303,   13,   13, 0x0a,
     343,   13,   13,   13, 0x0a,
     355,   13,   13,   13, 0x0a,
     380,   13,   13,   13, 0x09,
     409,  399,  394,   13, 0x09,
     430,   13,   13,   13, 0x09,
     448,   13,   13,   13, 0x08,
     467,   13,   13,   13, 0x08,

 // properties: name, type, flags
     478,  394, 0x01094103,
     497,  394, 0x01095103,
     512,  394, 0x01095103,
     526,  394, 0x01095103,
     545,  394, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_Q3MainWindow[] = {
    "Q3MainWindow\0\0pixmapSizeChanged(bool)\0"
    "usesTextLabelChanged(bool)\0"
    "dockWindowPositionChanged(Q3DockWindow*)\0"
    "toolBarPositionChanged(Q3ToolBar*)\0"
    "setRightJustification(bool)\0"
    "setUsesBigPixmaps(bool)\0setUsesTextLabel(bool)\0"
    "setDockWindowsMovable(bool)\0"
    "setOpaqueMoving(bool)\0setDockMenuEnabled(bool)\0"
    "whatsThis()\0dw,a\0setAppropriate(Q3DockWindow*,bool)\0"
    "customize()\0setToolBarsMovable(bool)\0"
    "setUpLayout()\0bool\0globalPos\0"
    "showDockMenu(QPoint)\0menuAboutToShow()\0"
    "slotPlaceChanged()\0doLineUp()\0"
    "rightJustification\0usesBigPixmaps\0"
    "usesTextLabel\0dockWindowsMovable\0"
    "opaqueMoving\0"
};

void Q3MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Q3MainWindow *_t = static_cast<Q3MainWindow *>(_o);
        switch (_id) {
        case 0: _t->pixmapSizeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->usesTextLabelChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->dockWindowPositionChanged((*reinterpret_cast< Q3DockWindow*(*)>(_a[1]))); break;
        case 3: _t->toolBarPositionChanged((*reinterpret_cast< Q3ToolBar*(*)>(_a[1]))); break;
        case 4: _t->setRightJustification((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setUsesBigPixmaps((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setUsesTextLabel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setDockWindowsMovable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setOpaqueMoving((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->setDockMenuEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->whatsThis(); break;
        case 11: _t->setAppropriate((*reinterpret_cast< Q3DockWindow*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->customize(); break;
        case 13: _t->setToolBarsMovable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setUpLayout(); break;
        case 15: { bool _r = _t->showDockMenu((*reinterpret_cast< const QPoint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: _t->menuAboutToShow(); break;
        case 17: _t->slotPlaceChanged(); break;
        case 18: _t->doLineUp(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Q3MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Q3MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Q3MainWindow,
      qt_meta_data_Q3MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Q3MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Q3MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Q3MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Q3MainWindow))
        return static_cast<void*>(const_cast< Q3MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int Q3MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = rightJustification(); break;
        case 1: *reinterpret_cast< bool*>(_v) = usesBigPixmaps(); break;
        case 2: *reinterpret_cast< bool*>(_v) = usesTextLabel(); break;
        case 3: *reinterpret_cast< bool*>(_v) = dockWindowsMovable(); break;
        case 4: *reinterpret_cast< bool*>(_v) = opaqueMoving(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setRightJustification(*reinterpret_cast< bool*>(_v)); break;
        case 1: setUsesBigPixmaps(*reinterpret_cast< bool*>(_v)); break;
        case 2: setUsesTextLabel(*reinterpret_cast< bool*>(_v)); break;
        case 3: setDockWindowsMovable(*reinterpret_cast< bool*>(_v)); break;
        case 4: setOpaqueMoving(*reinterpret_cast< bool*>(_v)); break;
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

// SIGNAL 0
void Q3MainWindow::pixmapSizeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Q3MainWindow::usesTextLabelChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Q3MainWindow::dockWindowPositionChanged(Q3DockWindow * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Q3MainWindow::toolBarPositionChanged(Q3ToolBar * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
