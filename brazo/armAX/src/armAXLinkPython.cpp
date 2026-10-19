/**
 * @file armAXLinkPython.h
 * @brief Configuracion del modulo python para la calse armAX.
 * @author Jose Manuel Plascencia Ramos
 * @date 2025-04-01
 * @version 1.0
 */
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "armAX.h"
#include "DynamixelManager.h"

namespace py = pybind11;

PYBIND11_MODULE(armAX, m) {
    py::class_<armAX>(m, "armAX")
        .def(py::init<DynamixelManager*>())
        .def("get_messages", &armAX::get_messages)
        .def("connect", &armAX::connect, py::arg("home") = false)
        .def("setInHomePosition", &armAX::setInHomePosition)
        .def("setOnePosition", &armAX::setOnePosition)
        .def("setAllPosition", &armAX::setAllPosition)
        .def("setPositionArray", &armAX::setPositionArray)
        .def("getOnePosition", &armAX::getOnePosition)
        .def("getAllPosition", &armAX::getAllPosition)
        .def("getPositionArray", &armAX::getPositionArray);
}