/****************************************************************************
** Meta object code from reading C++ file 'audiodevices.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../audiodevices.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiodevices.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AudioTest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x08,
      32,   11,   10,   10, 0x08,
      51,   11,   10,   10, 0x08,
      68,   11,   10,   10, 0x08,
      88,   11,   10,   10, 0x08,
     106,   11,   10,   10, 0x08,
     129,   11,   10,   10, 0x08,
     152,   11,   10,   10, 0x08,
     171,   10,   10,   10, 0x08,
     178,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AudioTest[] = {
    "AudioTest\0\0idx\0modeChanged(int)\0"
    "deviceChanged(int)\0freqChanged(int)\0"
    "channelChanged(int)\0codecChanged(int)\0"
    "sampleSizeChanged(int)\0sampleTypeChanged(int)\0"
    "endianChanged(int)\0test()\0populateTable()\0"
};

void AudioTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AudioTest *_t = static_cast<AudioTest *>(_o);
        switch (_id) {
        case 0: _t->modeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->deviceChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->freqChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->channelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->codecChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->sampleSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->sampleTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->endianChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->test(); break;
        case 9: _t->populateTable(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AudioTest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AudioTest::staticMetaObject = {
    { &AudioDevicesBase::staticMetaObject, qt_meta_stringdata_AudioTest,
      qt_meta_data_AudioTest, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AudioTest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AudioTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AudioTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioTest))
        return static_cast<void*>(const_cast< AudioTest*>(this));
    return AudioDevicesBase::qt_metacast(_clname);
}

int AudioTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AudioDevicesBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
