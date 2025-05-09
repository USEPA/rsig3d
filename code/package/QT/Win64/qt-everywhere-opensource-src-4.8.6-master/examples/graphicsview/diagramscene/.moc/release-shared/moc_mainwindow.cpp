/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x08,
      69,   66,   11,   11, 0x08,
      93,   11,   11,   11, 0x08,
     106,   66,   11,   11, 0x08,
     131,   11,   11,   11, 0x08,
     146,   11,   11,   11, 0x08,
     164,  159,   11,   11, 0x08,
     191,  159,   11,   11, 0x08,
     229,  224,   11,   11, 0x08,
     260,  255,   11,   11, 0x08,
     291,  285,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     337,   11,   11,   11, 0x08,
     356,   11,   11,   11, 0x08,
     375,   11,   11,   11, 0x08,
     397,   11,   11,   11, 0x08,
     419,   11,   11,   11, 0x08,
     441,   11,   11,   11, 0x08,
     460,  159,   11,   11, 0x08,
     489,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0button\0"
    "backgroundButtonGroupClicked(QAbstractButton*)\0"
    "id\0buttonGroupClicked(int)\0deleteItem()\0"
    "pointerGroupClicked(int)\0bringToFront()\0"
    "sendToBack()\0item\0itemInserted(DiagramItem*)\0"
    "textInserted(QGraphicsTextItem*)\0font\0"
    "currentFontChanged(QFont)\0size\0"
    "fontSizeChanged(QString)\0scale\0"
    "sceneScaleChanged(QString)\0"
    "textColorChanged()\0itemColorChanged()\0"
    "lineColorChanged()\0textButtonTriggered()\0"
    "fillButtonTriggered()\0lineButtonTriggered()\0"
    "handleFontChange()\0itemSelected(QGraphicsItem*)\0"
    "about()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->backgroundButtonGroupClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 1: _t->buttonGroupClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->deleteItem(); break;
        case 3: _t->pointerGroupClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->bringToFront(); break;
        case 5: _t->sendToBack(); break;
        case 6: _t->itemInserted((*reinterpret_cast< DiagramItem*(*)>(_a[1]))); break;
        case 7: _t->textInserted((*reinterpret_cast< QGraphicsTextItem*(*)>(_a[1]))); break;
        case 8: _t->currentFontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 9: _t->fontSizeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->sceneScaleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->textColorChanged(); break;
        case 12: _t->itemColorChanged(); break;
        case 13: _t->lineColorChanged(); break;
        case 14: _t->textButtonTriggered(); break;
        case 15: _t->fillButtonTriggered(); break;
        case 16: _t->lineButtonTriggered(); break;
        case 17: _t->handleFontChange(); break;
        case 18: _t->itemSelected((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 19: _t->about(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
