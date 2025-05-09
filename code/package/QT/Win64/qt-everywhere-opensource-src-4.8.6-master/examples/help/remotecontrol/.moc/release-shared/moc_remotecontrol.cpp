/****************************************************************************
** Meta object code from reading C++ file 'remotecontrol.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../remotecontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'remotecontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RemoteControl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      41,   14,   14,   14, 0x08,
      67,   14,   14,   14, 0x08,
      92,   14,   14,   14, 0x08,
     122,   14,   14,   14, 0x08,
     145,   14,   14,   14, 0x08,
     185,  177,   14,   14, 0x08,
     219,  177,   14,   14, 0x08,
     250,  177,   14,   14, 0x08,
     285,   14,   14,   14, 0x08,
     308,  304,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RemoteControl[] = {
    "RemoteControl\0\0on_launchButton_clicked()\0"
    "on_actionQuit_triggered()\0"
    "on_indexButton_clicked()\0"
    "on_identifierButton_clicked()\0"
    "on_urlButton_clicked()\0"
    "on_syncContentsButton_clicked()\0checked\0"
    "on_contentsCheckBox_toggled(bool)\0"
    "on_indexCheckBox_toggled(bool)\0"
    "on_bookmarksCheckBox_toggled(bool)\0"
    "helpViewerClosed()\0cmd\0sendCommand(QString)\0"
};

void RemoteControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RemoteControl *_t = static_cast<RemoteControl *>(_o);
        switch (_id) {
        case 0: _t->on_launchButton_clicked(); break;
        case 1: _t->on_actionQuit_triggered(); break;
        case 2: _t->on_indexButton_clicked(); break;
        case 3: _t->on_identifierButton_clicked(); break;
        case 4: _t->on_urlButton_clicked(); break;
        case 5: _t->on_syncContentsButton_clicked(); break;
        case 6: _t->on_contentsCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_indexCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_bookmarksCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->helpViewerClosed(); break;
        case 10: _t->sendCommand((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RemoteControl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RemoteControl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RemoteControl,
      qt_meta_data_RemoteControl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RemoteControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RemoteControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RemoteControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteControl))
        return static_cast<void*>(const_cast< RemoteControl*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RemoteControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
