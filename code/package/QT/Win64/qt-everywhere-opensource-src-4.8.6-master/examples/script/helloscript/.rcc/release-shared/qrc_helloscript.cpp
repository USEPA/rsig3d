/****************************************************************************
** Resource object code
**
** Created by: The Resource Compiler for Qt version 4.8.6
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtCore/qglobal.h>

static const unsigned char qt_resource_data[] = {
  // /root/qt-everywhere-opensource-src-4.8.6/examples/script/helloscript/helloscript.js
  0x0,0x0,0x0,0x6d,
  0x2f,
  0x2f,0x21,0x20,0x5b,0x30,0x5d,0xa,0x62,0x75,0x74,0x74,0x6f,0x6e,0x2e,0x74,0x65,
  0x78,0x74,0x20,0x3d,0x20,0x71,0x73,0x54,0x72,0x28,0x27,0x48,0x65,0x6c,0x6c,0x6f,
  0x20,0x57,0x6f,0x72,0x6c,0x64,0x21,0x27,0x29,0x3b,0xa,0x62,0x75,0x74,0x74,0x6f,
  0x6e,0x2e,0x73,0x74,0x79,0x6c,0x65,0x53,0x68,0x65,0x65,0x74,0x20,0x3d,0x20,0x27,
  0x66,0x6f,0x6e,0x74,0x2d,0x73,0x74,0x79,0x6c,0x65,0x3a,0x20,0x69,0x74,0x61,0x6c,
  0x69,0x63,0x27,0x3b,0xa,0x62,0x75,0x74,0x74,0x6f,0x6e,0x2e,0x73,0x68,0x6f,0x77,
  0x28,0x29,0x3b,0xa,0x2f,0x2f,0x21,0x20,0x5b,0x30,0x5d,0xa,
  
};

static const unsigned char qt_resource_name[] = {
  // helloscript.js
  0x0,0xe,
  0x1,0x61,0xbd,0xf3,
  0x0,0x68,
  0x0,0x65,0x0,0x6c,0x0,0x6c,0x0,0x6f,0x0,0x73,0x0,0x63,0x0,0x72,0x0,0x69,0x0,0x70,0x0,0x74,0x0,0x2e,0x0,0x6a,0x0,0x73,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  // :/helloscript.js
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,

};

QT_BEGIN_NAMESPACE

extern Q_CORE_EXPORT bool qRegisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

extern Q_CORE_EXPORT bool qUnregisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

QT_END_NAMESPACE


int QT_MANGLE_NAMESPACE(qInitResources_helloscript)()
{
    QT_PREPEND_NAMESPACE(qRegisterResourceData)
        (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_helloscript))

int QT_MANGLE_NAMESPACE(qCleanupResources_helloscript)()
{
    QT_PREPEND_NAMESPACE(qUnregisterResourceData)
       (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_DESTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qCleanupResources_helloscript))

