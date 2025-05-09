/****************************************************************************
** Meta object code from reading C++ file 'imageanalyzer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imageanalyzer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imageanalyzer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ImageAnalyzer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       4,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      50,   34,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   74,   14,   14, 0x0a,
     106,   14,   14,   14, 0x08,
     134,   14,   14,   14, 0x08,
     151,   14,   14,   14, 0x08,

 // properties: name, type, flags
     176,  171, 0x01095001,
     187,  181, 0x87095001,
     191,  181, 0x87095001,
     197,  181, 0x87095001,

       0        // eod
};

static const char qt_meta_stringdata_ImageAnalyzer[] = {
    "ImageAnalyzer\0\0finishedAnalysis()\0"
    "completed,total\0updateProgress(int,int)\0"
    "urls\0startAnalysis(QStringList)\0"
    "handleReply(QNetworkReply*)\0"
    "doneProcessing()\0progressStatus(int)\0"
    "bool\0busy\0float\0red\0green\0blue\0"
};

void ImageAnalyzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ImageAnalyzer *_t = static_cast<ImageAnalyzer *>(_o);
        switch (_id) {
        case 0: _t->finishedAnalysis(); break;
        case 1: _t->updateProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->startAnalysis((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->handleReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: _t->doneProcessing(); break;
        case 5: _t->progressStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ImageAnalyzer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ImageAnalyzer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ImageAnalyzer,
      qt_meta_data_ImageAnalyzer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ImageAnalyzer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ImageAnalyzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ImageAnalyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ImageAnalyzer))
        return static_cast<void*>(const_cast< ImageAnalyzer*>(this));
    return QObject::qt_metacast(_clname);
}

int ImageAnalyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isBusy(); break;
        case 1: *reinterpret_cast< float*>(_v) = lastRed(); break;
        case 2: *reinterpret_cast< float*>(_v) = lastGreen(); break;
        case 3: *reinterpret_cast< float*>(_v) = lastBlue(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ImageAnalyzer::finishedAnalysis()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ImageAnalyzer::updateProgress(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
