/****************************************************************************
** Meta object code from reading C++ file 'serveur.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ServeurObjetsReel/serveur.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serveur.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Serveur_t {
    QByteArrayData data[10];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Serveur_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Serveur_t qt_meta_stringdata_Serveur = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Serveur"
QT_MOC_LITERAL(1, 8, 13), // "newConnection"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 15), // "connectionClose"
QT_MOC_LITERAL(4, 39, 8), // "received"
QT_MOC_LITERAL(5, 48, 5), // "char*"
QT_MOC_LITERAL(6, 54, 4), // "buff"
QT_MOC_LITERAL(7, 59, 3), // "len"
QT_MOC_LITERAL(8, 63, 20), // "launchListenerThread"
QT_MOC_LITERAL(9, 84, 4) // "port"

    },
    "Serveur\0newConnection\0\0connectionClose\0"
    "received\0char*\0buff\0len\0launchListenerThread\0"
    "port"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Serveur[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    2,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   41,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::UShort,    9,

       0        // eod
};

void Serveur::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Serveur *_t = static_cast<Serveur *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newConnection(); break;
        case 1: _t->connectionClose(); break;
        case 2: _t->received((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->launchListenerThread((*reinterpret_cast< unsigned short(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Serveur::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Serveur::newConnection)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Serveur::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Serveur::connectionClose)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Serveur::*)(char * , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Serveur::received)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Serveur::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Serveur.data,
    qt_meta_data_Serveur,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Serveur::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Serveur::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Serveur.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Serveur::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Serveur::newConnection()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Serveur::connectionClose()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Serveur::received(char * _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
