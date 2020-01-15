/****************************************************************************
** Meta object code from reading C++ file 'cdownloader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataCollectionService/cdownloader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cdownloader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CDownLoader_t {
    QByteArrayData data[11];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CDownLoader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CDownLoader_t qt_meta_stringdata_CDownLoader = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CDownLoader"
QT_MOC_LITERAL(1, 12, 7), // "onReady"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 11), // "resultReady"
QT_MOC_LITERAL(4, 33, 6), // "result"
QT_MOC_LITERAL(5, 40, 7), // "getData"
QT_MOC_LITERAL(6, 48, 3), // "url"
QT_MOC_LITERAL(7, 52, 8), // "onResult"
QT_MOC_LITERAL(8, 61, 14), // "QNetworkReply*"
QT_MOC_LITERAL(9, 76, 5), // "reply"
QT_MOC_LITERAL(10, 82, 6) // "doWork"

    },
    "CDownLoader\0onReady\0\0resultReady\0"
    "result\0getData\0url\0onResult\0QNetworkReply*\0"
    "reply\0doWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CDownLoader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   43,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
      10,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void CDownLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CDownLoader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onReady(); break;
        case 1: _t->resultReady((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->getData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onResult((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: _t->doWork((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CDownLoader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CDownLoader::onReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CDownLoader::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CDownLoader::resultReady)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CDownLoader::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CDownLoader.data,
    qt_meta_data_CDownLoader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CDownLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CDownLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CDownLoader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CDownLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CDownLoader::onReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CDownLoader::resultReady(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
