/****************************************************************************
** Meta object code from reading C++ file 'FileOrientedDownloaderWorker.h'
**
** Created: Thu May 8 12:58:26 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FileOrientedDownloaderWorker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileOrientedDownloaderWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileOrientedDownloaderWorker[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x05,
      47,   29,   29,   29, 0x05,
      71,   62,   29,   29, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_FileOrientedDownloaderWorker[] = {
    "FileOrientedDownloaderWorker\0\0"
    "fileDownloaded()\0finished(bool)\0"
    "numFiles\0numFilesSignal(long)\0"
};

void FileOrientedDownloaderWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileOrientedDownloaderWorker *_t = static_cast<FileOrientedDownloaderWorker *>(_o);
        switch (_id) {
        case 0: _t->fileDownloaded(); break;
        case 1: _t->finished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->numFilesSignal((*reinterpret_cast< long(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileOrientedDownloaderWorker::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileOrientedDownloaderWorker::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_FileOrientedDownloaderWorker,
      qt_meta_data_FileOrientedDownloaderWorker, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileOrientedDownloaderWorker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileOrientedDownloaderWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileOrientedDownloaderWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileOrientedDownloaderWorker))
        return static_cast<void*>(const_cast< FileOrientedDownloaderWorker*>(this));
    return QThread::qt_metacast(_clname);
}

int FileOrientedDownloaderWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FileOrientedDownloaderWorker::fileDownloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FileOrientedDownloaderWorker::finished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileOrientedDownloaderWorker::numFilesSignal(long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
