/****************************************************************************
** Meta object code from reading C++ file 'browsermainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../browsermainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'browsermainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BrowserMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      44,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x0a,
      41,   18,   18,   18, 0x0a,
      52,   18,   18,   18, 0x08,
      59,   18,   18,   18, 0x08,
      88,   81,   18,   18, 0x08,
     123,  117,   18,   18, 0x08,
     154,   18,   18,   18, 0x28,
     178,   19,   18,   18, 0x08,
     192,   18,   18,   18, 0x08,
     210,   18,   18,   18, 0x08,
     224,   18,   18,   18, 0x08,
     239,   18,   18,   18, 0x08,
     262,   18,   18,   18, 0x08,
     278,   18,   18,   18, 0x08,
     300,   18,   18,   18, 0x08,
     317,   18,   18,   18, 0x08,
     332,   18,   18,   18, 0x08,
     351,   18,   18,   18, 0x08,
     374,   18,   18,   18, 0x08,
     400,   18,   18,   18, 0x08,
     418,   18,   18,   18, 0x08,
     435,   18,   18,   18, 0x08,
     453,   18,   18,   18, 0x08,
     480,  473,   18,   18, 0x08,
     507,   18,   18,   18, 0x08,
     525,   18,   18,   18, 0x08,
     548,   18,   18,   18, 0x08,
     568,   18,   18,   18, 0x08,
     589,  473,   18,   18, 0x08,
     614,   18,   18,   18, 0x08,
     630,  473,   18,   18, 0x08,
     656,   18,   18,   18, 0x08,
     679,   18,   18,   18, 0x08,
     701,   18,   18,   18, 0x08,
     722,   18,   18,   18, 0x08,
     748,   18,   18,   18, 0x08,
     777,   18,   18,   18, 0x08,
     812,  805,   18,   18, 0x08,
     840,   18,   18,   18, 0x08,
     857,   18,   18,   18, 0x08,
     879,  873,   18,   18, 0x08,
     915,  906,   18,   18, 0x08,
     954,  946,   18,   18, 0x08,
     984,  946,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BrowserMainWindow[] = {
    "BrowserMainWindow\0\0url\0loadPage(QString)\0"
    "slotHome()\0save()\0slotLoadProgress(int)\0"
    "string\0slotUpdateStatusbar(QString)\0"
    "title\0slotUpdateWindowTitle(QString)\0"
    "slotUpdateWindowTitle()\0loadUrl(QUrl)\0"
    "slotPreferences()\0slotFileNew()\0"
    "slotFileOpen()\0slotFilePrintPreview()\0"
    "slotFilePrint()\0slotPrivateBrowsing()\0"
    "slotFileSaveAs()\0slotEditFind()\0"
    "slotEditFindNext()\0slotEditFindPrevious()\0"
    "slotShowBookmarksDialog()\0slotAddBookmark()\0"
    "slotViewZoomIn()\0slotViewZoomOut()\0"
    "slotViewResetZoom()\0enable\0"
    "slotViewZoomTextOnly(bool)\0slotViewToolbar()\0"
    "slotViewBookmarksBar()\0slotViewStatusbar()\0"
    "slotViewPageSource()\0slotViewFullScreen(bool)\0"
    "slotWebSearch()\0slotToggleInspector(bool)\0"
    "slotAboutApplication()\0slotDownloadManager()\0"
    "slotSelectLineEdit()\0slotAboutToShowBackMenu()\0"
    "slotAboutToShowForwardMenu()\0"
    "slotAboutToShowWindowMenu()\0action\0"
    "slotOpenActionUrl(QAction*)\0"
    "slotShowWindow()\0slotSwapFocus()\0frame\0"
    "printRequested(QWebFrame*)\0geometry\0"
    "geometryChangeRequested(QRect)\0visible\0"
    "updateToolbarActionText(bool)\0"
    "updateBookmarksToolbarActionText(bool)\0"
};

void BrowserMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BrowserMainWindow *_t = static_cast<BrowserMainWindow *>(_o);
        switch (_id) {
        case 0: _t->loadPage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->slotHome(); break;
        case 2: _t->save(); break;
        case 3: _t->slotLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotUpdateStatusbar((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->slotUpdateWindowTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->slotUpdateWindowTitle(); break;
        case 7: _t->loadUrl((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 8: _t->slotPreferences(); break;
        case 9: _t->slotFileNew(); break;
        case 10: _t->slotFileOpen(); break;
        case 11: _t->slotFilePrintPreview(); break;
        case 12: _t->slotFilePrint(); break;
        case 13: _t->slotPrivateBrowsing(); break;
        case 14: _t->slotFileSaveAs(); break;
        case 15: _t->slotEditFind(); break;
        case 16: _t->slotEditFindNext(); break;
        case 17: _t->slotEditFindPrevious(); break;
        case 18: _t->slotShowBookmarksDialog(); break;
        case 19: _t->slotAddBookmark(); break;
        case 20: _t->slotViewZoomIn(); break;
        case 21: _t->slotViewZoomOut(); break;
        case 22: _t->slotViewResetZoom(); break;
        case 23: _t->slotViewZoomTextOnly((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->slotViewToolbar(); break;
        case 25: _t->slotViewBookmarksBar(); break;
        case 26: _t->slotViewStatusbar(); break;
        case 27: _t->slotViewPageSource(); break;
        case 28: _t->slotViewFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: _t->slotWebSearch(); break;
        case 30: _t->slotToggleInspector((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: _t->slotAboutApplication(); break;
        case 32: _t->slotDownloadManager(); break;
        case 33: _t->slotSelectLineEdit(); break;
        case 34: _t->slotAboutToShowBackMenu(); break;
        case 35: _t->slotAboutToShowForwardMenu(); break;
        case 36: _t->slotAboutToShowWindowMenu(); break;
        case 37: _t->slotOpenActionUrl((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 38: _t->slotShowWindow(); break;
        case 39: _t->slotSwapFocus(); break;
        case 40: _t->printRequested((*reinterpret_cast< QWebFrame*(*)>(_a[1]))); break;
        case 41: _t->geometryChangeRequested((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 42: _t->updateToolbarActionText((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 43: _t->updateBookmarksToolbarActionText((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BrowserMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BrowserMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BrowserMainWindow,
      qt_meta_data_BrowserMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BrowserMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BrowserMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BrowserMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BrowserMainWindow))
        return static_cast<void*>(const_cast< BrowserMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BrowserMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 44)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 44;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
