/****************************************************************************
** Meta object code from reading C++ file 'qscriptdebuggercodewidget_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../debugging/qscriptdebuggercodewidget_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscriptdebuggercodewidget_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QScriptDebuggerCodeWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   27,   26,   26, 0x08,
      68,   27,   26,   26, 0x08,
     110,  107,   26,   26, 0x08,
     164,  107,   26,   26, 0x08,
     210,   27,   26,   26, 0x08,
     263,   26,   26,   26, 0x08,
     285,  107,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QScriptDebuggerCodeWidget[] = {
    "QScriptDebuggerCodeWidget\0\0,\0"
    "_q_onBreakpointToggleRequest(int,bool)\0"
    "_q_onBreakpointEnableRequest(int,bool)\0"
    ",,\0_q_onBreakpointsAboutToBeRemoved(QModelIndex,int,int)\0"
    "_q_onBreakpointsInserted(QModelIndex,int,int)\0"
    "_q_onBreakpointsDataChanged(QModelIndex,QModelIndex)\0"
    "_q_onScriptsChanged()\0"
    "_q_onToolTipRequest(QPoint,int,QStringList)\0"
};

void QScriptDebuggerCodeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QScriptDebuggerCodeWidget *_t = static_cast<QScriptDebuggerCodeWidget *>(_o);
        switch (_id) {
        case 0: _t->d_func()->_q_onBreakpointToggleRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->d_func()->_q_onBreakpointEnableRequest((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->d_func()->_q_onBreakpointsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->d_func()->_q_onBreakpointsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->d_func()->_q_onBreakpointsDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 5: _t->d_func()->_q_onScriptsChanged(); break;
        case 6: _t->d_func()->_q_onToolTipRequest((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QScriptDebuggerCodeWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QScriptDebuggerCodeWidget::staticMetaObject = {
    { &QScriptDebuggerCodeWidgetInterface::staticMetaObject, qt_meta_stringdata_QScriptDebuggerCodeWidget,
      qt_meta_data_QScriptDebuggerCodeWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QScriptDebuggerCodeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QScriptDebuggerCodeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QScriptDebuggerCodeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QScriptDebuggerCodeWidget))
        return static_cast<void*>(const_cast< QScriptDebuggerCodeWidget*>(this));
    return QScriptDebuggerCodeWidgetInterface::qt_metacast(_clname);
}

int QScriptDebuggerCodeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScriptDebuggerCodeWidgetInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
