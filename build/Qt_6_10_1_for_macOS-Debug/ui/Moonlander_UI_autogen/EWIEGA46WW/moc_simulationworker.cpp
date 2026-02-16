/****************************************************************************
** Meta object code from reading C++ file 'simulationworker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
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
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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

template <> constexpr inline auto SimulationWorker::qt_create_metaobjectdata<qt_meta_tag_ZN16SimulationWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
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
        "setAutopilotFlag",
        "active",
        "stepSimulation"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'stateUpdated'
        QtMocHelpers::SignalData<void(double, Vector3, Vector3, double, SpacecraftState, double, double, double, double)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 3 }, { 0x80000000 | 4, 5 }, { 0x80000000 | 4, 6 }, { QMetaType::Double, 7 },
            { 0x80000000 | 8, 9 }, { QMetaType::Double, 10 }, { QMetaType::Double, 11 }, { QMetaType::Double, 12 },
            { QMetaType::Double, 13 },
        }}),
        // Signal 'simulationError'
        QtMocHelpers::SignalData<void(QString)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'start'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'pause'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setTargetThrust'
        QtMocHelpers::SlotData<void(double)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 20 },
        }}),
        // Slot 'receiveJsonConfig'
        QtMocHelpers::SlotData<void(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 22 },
        }}),
        // Slot 'setAutopilotFlag'
        QtMocHelpers::SlotData<void(bool)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 24 },
        }}),
        // Slot 'stepSimulation'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SimulationWorker, qt_meta_tag_ZN16SimulationWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SimulationWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SimulationWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SimulationWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16SimulationWorkerE_t>.metaTypes,
    nullptr
} };

void SimulationWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SimulationWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stateUpdated((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Vector3>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<Vector3>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<SpacecraftState>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[9]))); break;
        case 1: _t->simulationError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->start(); break;
        case 3: _t->pause(); break;
        case 4: _t->stop(); break;
        case 5: _t->setTargetThrust((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->receiveJsonConfig((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->setAutopilotFlag((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->stepSimulation(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SimulationWorker::*)(double , Vector3 , Vector3 , double , SpacecraftState , double , double , double , double )>(_a, &SimulationWorker::stateUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SimulationWorker::*)(QString )>(_a, &SimulationWorker::simulationError, 1))
            return;
    }
}

const QMetaObject *SimulationWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimulationWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16SimulationWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SimulationWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void SimulationWorker::stateUpdated(double _t1, Vector3 _t2, Vector3 _t3, double _t4, SpacecraftState _t5, double _t6, double _t7, double _t8, double _t9)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9);
}

// SIGNAL 1
void SimulationWorker::simulationError(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
