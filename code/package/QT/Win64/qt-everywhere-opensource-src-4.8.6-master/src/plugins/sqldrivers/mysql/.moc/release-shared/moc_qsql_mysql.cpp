/****************************************************************************
** Meta object code from reading C++ file 'qsql_mysql.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../sql/drivers/mysql/qsql_mysql.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsql_mysql.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMYSQLDriver[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      35,   19,   14,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_QMYSQLDriver[] = {
    "QMYSQLDriver\0\0bool\0identifier,type\0"
    "isIdentifierEscapedImplementation(QString,IdentifierType)\0"
};

void QMYSQLDriver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMYSQLDriver *_t = static_cast<QMYSQLDriver *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->isIdentifierEscapedImplementation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< IdentifierType(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QMYSQLDriver::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMYSQLDriver::staticMetaObject = {
    { &QSqlDriver::staticMetaObject, qt_meta_stringdata_QMYSQLDriver,
      qt_meta_data_QMYSQLDriver, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMYSQLDriver::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMYSQLDriver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMYSQLDriver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMYSQLDriver))
        return static_cast<void*>(const_cast< QMYSQLDriver*>(this));
    return QSqlDriver::qt_metacast(_clname);
}

int QMYSQLDriver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSqlDriver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
