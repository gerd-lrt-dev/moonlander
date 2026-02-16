/****************************************************************************
** Meta object code from reading C++ file 'cockpitpage.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../ui/cockpitpage.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cockpitpage.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11cockpitPageE_t {};
} // unnamed namespace

template <> constexpr inline auto cockpitPage::qt_create_metaobjectdata<qt_meta_tag_ZN11cockpitPageE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cockpitPage",
        "startRequested",
        "",
        "pauseRequested",
        "stopConfirmed",
        "resetSimulationRequested",
        "thrustTargetRequested",
        "percent",
        "autopilotToggled",
        "acitve",
        "onStateUpdated",
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
        "onStopClicked",
        "onAutopilotClicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'startRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pauseRequested'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stopConfirmed'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'resetSimulationRequested'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'thrustTargetRequested'
        QtMocHelpers::SignalData<void(double)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 7 },
        }}),
        // Signal 'autopilotToggled'
        QtMocHelpers::SignalData<void(bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'onStateUpdated'
        QtMocHelpers::SlotData<void(double, const Vector3 &, const Vector3 &, const double &, SpacecraftState, double, double, double, double)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 11 }, { 0x80000000 | 12, 13 }, { 0x80000000 | 12, 14 }, { QMetaType::Double, 15 },
            { 0x80000000 | 16, 17 }, { QMetaType::Double, 18 }, { QMetaType::Double, 19 }, { QMetaType::Double, 20 },
            { QMetaType::Double, 21 },
        }}),
        // Slot 'onStopClicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAutopilotClicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<cockpitPage, qt_meta_tag_ZN11cockpitPageE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cockpitPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11cockpitPageE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11cockpitPageE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11cockpitPageE_t>.metaTypes,
    nullptr
} };

void cockpitPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<cockpitPage *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->startRequested(); break;
        case 1: _t->pauseRequested(); break;
        case 2: _t->stopConfirmed(); break;
        case 3: _t->resetSimulationRequested(); break;
        case 4: _t->thrustTargetRequested((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 5: _t->autopilotToggled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 6: _t->onStateUpdated((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Vector3>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<Vector3>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<SpacecraftState>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[8])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[9]))); break;
        case 7: _t->onStopClicked(); break;
        case 8: _t->onAutopilotClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)()>(_a, &cockpitPage::startRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)()>(_a, &cockpitPage::pauseRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)()>(_a, &cockpitPage::stopConfirmed, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)()>(_a, &cockpitPage::resetSimulationRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)(double )>(_a, &cockpitPage::thrustTargetRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (cockpitPage::*)(bool )>(_a, &cockpitPage::autopilotToggled, 5))
            return;
    }
}

const QMetaObject *cockpitPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cockpitPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11cockpitPageE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int cockpitPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void cockpitPage::startRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void cockpitPage::pauseRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void cockpitPage::stopConfirmed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void cockpitPage::resetSimulationRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void cockpitPage::thrustTargetRequested(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void cockpitPage::autopilotToggled(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
