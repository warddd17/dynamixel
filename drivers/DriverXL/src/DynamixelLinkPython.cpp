/**
 * @file DynamixelLinkPython.cpp
 * @brief Módulo Python para control de motores Dynamixel (vía pybind11).
 * @details Expone las clases DynamixelManager y DynamixelAXControl a Python.
 * @warning Para uso unicamente con protocolo 1.0. 
 */

#include <pybind11/pybind11.h>
#include "DynamixelAXControl.h"
#include "DynamixelManager.h"

namespace py = pybind11;

PYBIND11_MODULE(DynamixelAXControl,m){
    py::class_<DynamixelAXControl>(m , "DynamixelAXControl")
        .def(py::init<DynamixelManager* , int >())
        .def("get_message",&DynamixelAXControl::get_message)
        .def("get_id",&DynamixelAXControl::get_id)
        .def("connect",&DynamixelAXControl::connect, py::arg("home")=false)
        .def("setWheelMode",&DynamixelAXControl::setWheelMode)
        .def("setJointMode",&DynamixelAXControl::setJointMode, py::arg("cwLimit"), py::arg("ccwLimit"), py::arg("degrees")=false)
        .def("setPosition",&DynamixelAXControl::setPosition, py::arg("position"), py::arg("degrees")=false)
        .def("setPositionInTime",&DynamixelAXControl::setPositionInTime, py::arg("position"), py::arg("time"), py::arg("degrees")=false)
        .def("setSpeed",&DynamixelAXControl::setSpeed)
        .def("setHome",&DynamixelAXControl::setHome)
        .def("getPosition",&DynamixelAXControl::getPosition)
        .def("getSpeed",&DynamixelAXControl::getSpeed)
        .def("getVoltaje",&DynamixelAXControl::getVoltaje)
        .def("getTemperature",&DynamixelAXControl::getTemperature)
        .def("isMoving",&DynamixelAXControl::isMoving);
}