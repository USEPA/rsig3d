/****************************************************************************
** Meta object code from reading C++ file 'tabwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tabwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tabwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TabBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      23,   17,    7,    7, 0x05,
      37,   17,    7,    7, 0x05,
      51,   17,    7,    7, 0x05,
      71,   17,    7,    7, 0x05,
      86,    7,    7,    7, 0x05,
     120,  102,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     146,    7,    7,    7, 0x08,
     164,    7,    7,    7, 0x08,
     175,    7,    7,    7, 0x08,
     186,    7,    7,    7, 0x08,
     203,    7,    7,    7, 0x08,
     224,  215,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TabBar[] = {
    "TabBar\0\0newTab()\0index\0cloneTab(int)\0"
    "closeTab(int)\0closeOtherTabs(int)\0"
    "reloadTab(int)\0reloadAllTabs()\0"
    "fromIndex,toIndex\0tabMoveRequested(int,int)\0"
    "selectTabAction()\0cloneTab()\0closeTab()\0"
    "closeOtherTabs()\0reloadTab()\0position\0"
    "contextMenuRequested(QPoint)\0"
};

void TabBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TabBar *_t = static_cast<TabBar *>(_o);
        switch (_id) {
        case 0: _t->newTab(); break;
        case 1: _t->cloneTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->closeOtherTabs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->reloadTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->reloadAllTabs(); break;
        case 6: _t->tabMoveRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->selectTabAction(); break;
        case 8: _t->cloneTab(); break;
        case 9: _t->closeTab(); break;
        case 10: _t->closeOtherTabs(); break;
        case 11: _t->reloadTab(); break;
        case 12: _t->contextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TabBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TabBar::staticMetaObject = {
    { &QTabBar::staticMetaObject, qt_meta_stringdata_TabBar,
      qt_meta_data_TabBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TabBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TabBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TabBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabBar))
        return static_cast<void*>(const_cast< TabBar*>(this));
    return QTabBar::qt_metacast(_clname);
}

int TabBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void TabBar::newTab()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void TabBar::cloneTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TabBar::closeTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TabBar::closeOtherTabs(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TabBar::reloadTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TabBar::reloadAllTabs()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void TabBar::tabMoveRequested(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
static const uint qt_meta_data_WebActionMapper[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      33,   16,   16,   16, 0x08,
      48,   16,   16,   16, 0x08,
      64,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WebActionMapper[] = {
    "WebActionMapper\0\0rootTriggered()\0"
    "childChanged()\0rootDestroyed()\0"
    "currentDestroyed()\0"
};

void WebActionMapper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WebActionMapper *_t = static_cast<WebActionMapper *>(_o);
        switch (_id) {
        case 0: _t->rootTriggered(); break;
        case 1: _t->childChanged(); break;
        case 2: _t->rootDestroyed(); break;
        case 3: _t->currentDestroyed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData WebActionMapper::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WebActionMapper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WebActionMapper,
      qt_meta_data_WebActionMapper, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WebActionMapper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WebActionMapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WebActionMapper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WebActionMapper))
        return static_cast<void*>(const_cast< WebActionMapper*>(this));
    return QObject::qt_metacast(_clname);
}

int WebActionMapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_TabWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   11,   10,   10, 0x05,
      33,   10,   10,   10, 0x05,
      47,   10,   10,   10, 0x05,
      63,   11,   10,   10, 0x05,
      96,   88,   10,   10, 0x05,
     131,  126,   10,   10, 0x05,
     161,  152,   10,   10, 0x05,
     188,  179,   10,   10, 0x05,
     227,  219,   10,   10, 0x05,
     266,  219,   10,   10, 0x05,
     307,  219,   10,   10, 0x05,
     352,  346,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     379,   11,   10,   10, 0x0a,
     426,  414,  405,   10, 0x0a,
     439,   10,  405,   10, 0x2a,
     454,  448,   10,   10, 0x0a,
     468,   10,   10,   10, 0x2a,
     479,  448,   10,   10, 0x0a,
     493,   10,   10,   10, 0x2a,
     504,  448,   10,   10, 0x0a,
     524,  448,   10,   10, 0x0a,
     539,   10,   10,   10, 0x2a,
     551,   10,   10,   10, 0x0a,
     567,   10,   10,   10, 0x0a,
     577,   10,   10,   10, 0x0a,
     591,  448,   10,   10, 0x08,
     611,   10,   10,   10, 0x08,
     646,  639,   10,   10, 0x08,
     689,   10,   10,   10, 0x08,
     710,   10,   10,   10, 0x08,
     737,  731,   10,   10, 0x08,
     766,   11,   10,   10, 0x08,
     790,   10,   10,   10, 0x08,
     814,   10,   10,   10, 0x08,
     855,  837,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TabWidget[] = {
    "TabWidget\0\0url\0loadPage(QString)\0"
    "tabsChanged()\0lastTabClosed()\0"
    "setCurrentTitle(QString)\0message\0"
    "showStatusBarMessage(QString)\0link\0"
    "linkHovered(QString)\0progress\0"
    "loadProgress(int)\0geometry\0"
    "geometryChangeRequested(QRect)\0visible\0"
    "menuBarVisibilityChangeRequested(bool)\0"
    "statusBarVisibilityChangeRequested(bool)\0"
    "toolBarVisibilityChangeRequested(bool)\0"
    "frame\0printRequested(QWebFrame*)\0"
    "loadUrlInCurrentTab(QUrl)\0WebView*\0"
    "makeCurrent\0newTab(bool)\0newTab()\0"
    "index\0cloneTab(int)\0cloneTab()\0"
    "closeTab(int)\0closeTab()\0closeOtherTabs(int)\0"
    "reloadTab(int)\0reloadTab()\0reloadAllTabs()\0"
    "nextTab()\0previousTab()\0currentChanged(int)\0"
    "aboutToShowRecentTabsMenu()\0action\0"
    "aboutToShowRecentTriggeredAction(QAction*)\0"
    "webViewLoadStarted()\0webViewIconChanged()\0"
    "title\0webViewTitleChanged(QString)\0"
    "webViewUrlChanged(QUrl)\0lineEditReturnPressed()\0"
    "windowCloseRequested()\0fromIndex,toIndex\0"
    "moveTab(int,int)\0"
};

void TabWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TabWidget *_t = static_cast<TabWidget *>(_o);
        switch (_id) {
        case 0: _t->loadPage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->tabsChanged(); break;
        case 2: _t->lastTabClosed(); break;
        case 3: _t->setCurrentTitle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->showStatusBarMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->linkHovered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->loadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->geometryChangeRequested((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 8: _t->menuBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->statusBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->toolBarVisibilityChangeRequested((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->printRequested((*reinterpret_cast< QWebFrame*(*)>(_a[1]))); break;
        case 12: _t->loadUrlInCurrentTab((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 13: { WebView* _r = _t->newTab((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< WebView**>(_a[0]) = _r; }  break;
        case 14: { WebView* _r = _t->newTab();
            if (_a[0]) *reinterpret_cast< WebView**>(_a[0]) = _r; }  break;
        case 15: _t->cloneTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->cloneTab(); break;
        case 17: _t->closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->closeTab(); break;
        case 19: _t->closeOtherTabs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->reloadTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->reloadTab(); break;
        case 22: _t->reloadAllTabs(); break;
        case 23: _t->nextTab(); break;
        case 24: _t->previousTab(); break;
        case 25: _t->currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->aboutToShowRecentTabsMenu(); break;
        case 27: _t->aboutToShowRecentTriggeredAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 28: _t->webViewLoadStarted(); break;
        case 29: _t->webViewIconChanged(); break;
        case 30: _t->webViewTitleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 31: _t->webViewUrlChanged((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 32: _t->lineEditReturnPressed(); break;
        case 33: _t->windowCloseRequested(); break;
        case 34: _t->moveTab((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TabWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TabWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_TabWidget,
      qt_meta_data_TabWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TabWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TabWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TabWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabWidget))
        return static_cast<void*>(const_cast< TabWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int TabWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void TabWidget::loadPage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TabWidget::tabsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void TabWidget::lastTabClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void TabWidget::setCurrentTitle(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TabWidget::showStatusBarMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TabWidget::linkHovered(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TabWidget::loadProgress(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TabWidget::geometryChangeRequested(const QRect & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void TabWidget::menuBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void TabWidget::statusBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void TabWidget::toolBarVisibilityChangeRequested(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void TabWidget::printRequested(QWebFrame * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_END_MOC_NAMESPACE
