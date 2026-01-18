/****************************************************************************
** Meta object code from reading C++ file 'simulationworker.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../ui/simulationworker.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulationworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN16SimulationWorkerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN16SimulationWorkerE = QtMocHelpers::stringData(
    "SimulationWorker",
    "stateUpdated",
    "",
    "time",
    "Vector3",
    "pos",
    "vel",
    "GLoad",
    "SpacecraftState",
    "spacecraftState_",
    "thrust",
    "targetThrust",
    "fuelMass",
    "fuelFlow",
    "simulationError",
    "errorMsg",
    "start",
    "pause",
    "stop",
    "setTargetThrust",
    "percent",
    "receiveJsonConfig",
    "json",
    "stepSimulation"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN16SimulationWorkerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    9,   62,    2, 0x06,    1 /* Public */,
      14,    1,   81,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    0,   84,    2, 0x0a,   13 /* Public */,
      17,    0,   85,    2, 0x0a,   14 /* Public */,
      18,    0,   86,    2, 0x0a,   15 /* Public */,
      19,    1,   87,    2, 0x0a,   16 /* Public */,
      21,    1,   90,    2, 0x0a,   18 /* Public */,
      23,    0,   93,    2, 0x0a,   20 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, 0x80000000 | 4, 0x80000000 | 4, QMetaType::Double, 0x80000000 | 8, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,    3,    5,    6,    7,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::QString,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SimulationWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN16SimulationWorkerE.offsetsAndSizes,
    qt_meta_data_ZN16SimulationWorkerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN16SimulationWorkerE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SimulationWorker, std::true_type>,
        // method 'stateUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<Vector3, std::false_type>,
        QtPrivate::TypeAndForceComplete<Vector3, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<SpacecraftState, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'simulationError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'start'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pause'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setTargetThrust'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'receiveJsonConfig'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'stepSimulation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SimulationWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SimulationWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateUpdated((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Vector3>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<Vector3>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<SpacecraftState>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[9]))); break;
        case 1: _t->simulationError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->start(); break;
        case 3: _t->pause(); break;
        case 4: _t->stop(); break;
        case 5: _t->setTargetThrust((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->receiveJsonConfig((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->stepSimulation(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (SimulationWorker::*)(double , Vector3 , Vector3 , double , SpacecraftState , double , double , double , double );
            if (_q_method_type _q_method = &SimulationWorker::stateUpdated; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (SimulationWorker::*)(QString );
            if (_q_method_type _q_method = &SimulationWorker::simulationError; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *SimulationWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimulationWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN16SimulationWorkerE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SimulationWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SimulationWorker::stateUpdated(double _t1, Vector3 _t2, Vector3 _t3, double _t4, SpacecraftState _t5, double _t6, double _t7, double _t8, double _t9)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t8))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t9))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimulationWorker::simulationError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
