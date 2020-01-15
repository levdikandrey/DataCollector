/****************************************************************************
** Meta object code from reading C++ file 'daddtask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../arm_o/daddtask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'daddtask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DAddTask_t {
    QByteArrayData data[12];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DAddTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DAddTask_t qt_meta_stringdata_DAddTask = {
    {
QT_MOC_LITERAL(0, 0, 8), // "DAddTask"
QT_MOC_LITERAL(1, 9, 7), // "slotAdd"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 10), // "slotDelete"
QT_MOC_LITERAL(4, 29, 9), // "slotApply"
QT_MOC_LITERAL(5, 39, 10), // "slotCancel"
QT_MOC_LITERAL(6, 50, 8), // "slotExit"
QT_MOC_LITERAL(7, 59, 12), // "slotPrevious"
QT_MOC_LITERAL(8, 72, 8), // "slotNext"
QT_MOC_LITERAL(9, 81, 20), // "slotChangeNumberPage"
QT_MOC_LITERAL(10, 102, 11), // "slotFindAVP"
QT_MOC_LITERAL(11, 114, 15) // "slotTextChanged"

    },
    "DAddTask\0slotAdd\0\0slotDelete\0slotApply\0"
    "slotCancel\0slotExit\0slotPrevious\0"
    "slotNext\0slotChangeNumberPage\0slotFindAVP\0"
    "slotTextChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DAddTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    1,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void DAddTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DAddTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotAdd(); break;
        case 1: _t->slotDelete(); break;
        case 2: _t->slotApply(); break;
        case 3: _t->slotCancel(); break;
        case 4: _t->slotExit(); break;
        case 5: _t->slotPrevious(); break;
        case 6: _t->slotNext(); break;
        case 7: _t->slotChangeNumberPage(); break;
        case 8: _t->slotFindAVP(); break;
        case 9: _t->slotTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DAddTask::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_DAddTask.data,
    qt_meta_data_DAddTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DAddTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DAddTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DAddTask.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DAddTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
