/****************************************************************************
** Meta object code from reading C++ file 'FileOrientedDownloader.h'
**
** Created: Thu Jun 12 13:29:44 2025
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FileOrientedDownloader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileOrientedDownloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileOrientedDownloader[] = {

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
      24,   23,   23,   23, 0x09,
      45,   23,   23,   23, 0x09,
      65,   23,   23,   23, 0x09,
      83,   23,   23,   23, 0x09,
     105,   23,   23,   23, 0x09,
     130,   23,   23,   23, 0x09,
     149,   23,   23,   23, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_FileOrientedDownloader[] = {
    "FileOrientedDownloader\0\0onDirButtonClicked()\0"
    "onDownloadClicked()\0onCancelClicked()\0"
    "onListWidgetClicked()\0onDownloadFinished(bool)\0"
    "onFileDownloaded()\0onNumFilesChanged(long)\0"
};

void FileOrientedDownloader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileOrientedDownloader *_t = static_cast<FileOrientedDownloader *>(_o);
        switch (_id) {
        case 0: _t->onDirButtonClicked(); break;
        case 1: _t->onDownloadClicked(); break;
        case 2: _t->onCancelClicked(); break;
        case 3: _t->onListWidgetClicked(); break;
        case 4: _t->onDownloadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onFileDownloaded(); break;
        case 6: _t->onNumFilesChanged((*reinterpret_cast< long(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileOrientedDownloader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileOrientedDownloader::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FileOrientedDownloader,
      qt_meta_data_FileOrientedDownloader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileOrientedDownloader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileOrientedDownloader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileOrientedDownloader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileOrientedDownloader))
        return static_cast<void*>(const_cast< FileOrientedDownloader*>(this));
    return QDialog::qt_metacast(_clname);
}

int FileOrientedDownloader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
