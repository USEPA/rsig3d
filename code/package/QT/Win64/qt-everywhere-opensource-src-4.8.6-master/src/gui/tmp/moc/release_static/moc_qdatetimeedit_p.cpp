/****************************************************************************
** Meta object code from reading C++ file 'qdatetimeedit_p.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../widgets/qdatetimeedit_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdatetimeedit_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QCalendarPopup[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   16,   15,   15, 0x05,
      46,   38,   15,   15, 0x05,
      77,   69,   15,   15, 0x05,
      99,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     113,   16,   15,   15, 0x08,
     133,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QCalendarPopup[] = {
    "QCalendarPopup\0\0date\0activated(QDate)\0"
    "newDate\0newDateSelected(QDate)\0oldDate\0"
    "hidingCalendar(QDate)\0resetButton()\0"
    "dateSelected(QDate)\0dateSelectionChanged()\0"
};

void QCalendarPopup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QCalendarPopup *_t = static_cast<QCalendarPopup *>(_o);
        switch (_id) {
        case 0: _t->activated((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 1: _t->newDateSelected((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 2: _t->hidingCalendar((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 3: _t->resetButton(); break;
        case 4: _t->dateSelected((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 5: _t->dateSelectionChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QCalendarPopup::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QCalendarPopup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QCalendarPopup,
      qt_meta_data_QCalendarPopup, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QCalendarPopup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QCalendarPopup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QCalendarPopup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QCalendarPopup))
        return static_cast<void*>(const_cast< QCalendarPopup*>(this));
    return QWidget::qt_metacast(_clname);
}

int QCalendarPopup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QCalendarPopup::activated(const QDate & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QCalendarPopup::newDateSelected(const QDate & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QCalendarPopup::hidingCalendar(const QDate & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QCalendarPopup::resetButton()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
