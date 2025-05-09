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
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      53,   11,   11,   11, 0x0a,
      74,   11,   11,   11, 0x0a,
      97,   11,   11,   11, 0x0a,
     121,   11,   11,   11, 0x0a,
     149,   11,   11,   11, 0x0a,
     168,   11,   11,   11, 0x0a,
     184,   11,   11,   11, 0x0a,
     201,   11,   11,   11, 0x0a,
     216,   11,   11,   11, 0x0a,
     233,   11,   11,   11, 0x0a,
     247,   11,   11,   11, 0x0a,
     262,   11,   11,   11, 0x0a,
     275,   11,   11,   11, 0x0a,
     292,   11,   11,   11, 0x0a,
     303,   11,   11,   11, 0x0a,
     314,   11,   11,   11, 0x0a,
     334,   11,   11,   11, 0x0a,
     346,   11,   11,   11, 0x0a,
     360,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0buildPalette()\0buildFont()\0"
    "tunePalette()\0paletteSelected(int)\0"
    "styleSelected(QString)\0familySelected(QString)\0"
    "substituteSelected(QString)\0"
    "removeSubstitute()\0addSubstitute()\0"
    "downSubstitute()\0upSubstitute()\0"
    "removeFontpath()\0addFontpath()\0"
    "downFontpath()\0upFontpath()\0"
    "browseFontpath()\0fileSave()\0fileExit()\0"
    "somethingModified()\0helpAbout()\0"
    "helpAboutQt()\0pageChanged(int)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->buildPalette(); break;
        case 1: _t->buildFont(); break;
        case 2: _t->tunePalette(); break;
        case 3: _t->paletteSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->styleSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->familySelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->substituteSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->removeSubstitute(); break;
        case 8: _t->addSubstitute(); break;
        case 9: _t->downSubstitute(); break;
        case 10: _t->upSubstitute(); break;
        case 11: _t->removeFontpath(); break;
        case 12: _t->addFontpath(); break;
        case 13: _t->downFontpath(); break;
        case 14: _t->upFontpath(); break;
        case 15: _t->browseFontpath(); break;
        case 16: _t->fileSave(); break;
        case 17: _t->fileExit(); break;
        case 18: _t->somethingModified(); break;
        case 19: _t->helpAbout(); break;
        case 20: _t->helpAboutQt(); break;
        case 21: _t->pageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
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
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
