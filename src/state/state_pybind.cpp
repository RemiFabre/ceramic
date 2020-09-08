#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "factory.hpp"
#include "panel.hpp"
#include "pyramid.hpp"
#include "rules.hpp"
#include "state.hpp"
#include "tiles.hpp"
#include "wall.hpp"

namespace py = pybind11;

void state_pybind(py::module& m) {
    py::class_<Factory>(m, "Factory")
        .def(py::init<const ushort>())
        .def_readwrite("tiles", &Factory::tiles);



    py::class_<Panel>(m, "Panel")
        .def(py::init([](Rules rules){
            return std::make_shared<Rules>(rules);
        }))
        .def(py::init<const ushort>())
        .def(py::init<const Panel&>())

        .def_property_readonly("score", &Panel::get_score)
        .def_property_readonly("pyramid", &Panel::get_pyramid)
        .def_property_readonly("wall", &Panel::get_wall)
        .def_property_readonly("first_token", &Panel::get_first_token)
        .def_property_readonly("floor", &Panel::get_floor)
        .def_property_readonly("penalty", &Panel::get_penalty);



    py::class_<Pyramid>(m, "Pyramid")
        .def(py::init<const ushort>())
        .def(py::init<const Rules&>())
        .def(py::init<const Pyramid&>())

        .def("clear", &Pyramid::clear)
        .def("clear_line", &Pyramid::clear_line)
        .def("set_line", &Pyramid::set_line)

        .def("is_filled", &Pyramid::is_filled)
        .def("is_empty", &Pyramid::is_empty)
        .def("amount", &Pyramid::amount)
        .def("amount_remaining", &Pyramid::amount_remaining)
        .def("color", &Pyramid::color)
        .def("filled", &Pyramid::filled);



    py::class_<Rules>(m, "Rules")
        .def(py::init<>())
        .def(py::init<const Rules&>())

        .def_readwrite("player_count", &Rules::player_count)
        .def_readwrite("tile_count", &Rules::tile_count)
        .def_readwrite("tile_types", &Rules::tile_types)
        .def_readwrite("line_bonus", &Rules::line_bonus)
        .def_readwrite("column_bonus", &Rules::column_bonus)
        .def_readwrite("type_bonus", &Rules::type_bonus)
        .def_readwrite("overflow_count", &Rules::overflow_count)
        .def_readwrite("overflow_penalty", &Rules::overflow_penalty)

        .def("penalty_at", &Rules::penalty_at)
        .def("penalty_for_floor", &Rules::penalty_for_floor)
        .def("factories", &Rules::factories)
        
        .def_property_readonly_static("DEFAULT", &Rules::get_default);



    py::module tile_module = py::class_<Tile>(m, "Tile")
        .def(py::init<>())
        .def(py::init<const ushort>())
        .def(py::init<const Tile&>())

        .def("__int__", &Tile::operator int)
        .def("__eq__", [](const Tile& self, const Tile& other) { return self == other; })
        .def("__ne__", [](const Tile& self, const Tile& other) { return self != other; })
        .def("is_none", &Tile::is_none)
        .def("__str__", &Tile::str);
    tile_module.attr("NONE") = Tile::NONE;


    py::module tiles_module = py::class_<Tiles>(m, "Tiles")
        .def(py::init<>())
        .def(py::init<const Tile&>())
        .def(py::init<const vector<ushort>&>())
        .def(py::init<const Tiles&>())

        .def("__getitem__", [](Tiles& self, Tile tile) {
            return self[tile];
        })
        .def("__getitem__", [](Tiles& self, int tile) {
            return self[tile];
        })
        .def("__setitem__", [](Tiles& self, Tile tile, ushort value) {
            self[tile] = value;
        })
        .def("__setitem__", [](Tiles& self, int tile, ushort value) {
            self[tile] = value;
        })
        .def("total", &Tiles::total)
        .def("__len__", &Tiles::size)

        .def("__eq__", [](const Tiles& self, const Tiles& other) { return self == other; })
        .def("__ne__", [](const Tiles& self, const Tiles& other) { return self != other; })
        .def("__le__", [](const Tiles& self, const Tiles& other) { return self <= other; })
        .def("__ge__", [](const Tiles& self, const Tiles& other) { return self >= other; })
        
        .def("__add__", [](const Tiles& self, const Tiles& other) { return self + other; })
        .def("__sub__", [](const Tiles& self, const Tiles& other) { return self - other; })
        .def("__radd__", [](Tiles& self, const Tiles& other) { self += other; })
        .def("__rsub__", [](Tiles& self, const Tiles& other) { self -= other; })
        
        .def("__add__", [](const Tiles& self, const Tile& other) { return self + other; })
        .def("__sub__", [](const Tiles& self, const Tile& other) { return self - other; })
        .def("__radd__", [](Tiles& self, const Tile& other) { self += other; })
        .def("__rsub__", [](Tiles& self, const Tile& other) { self -= other; })
        
        .def("__str__", &Tiles::str)
        .def_property_readonly("quantities", &Tiles::get_quantities);
    tiles_module.attr("ZERO") = Tiles::ZERO;



    py::class_<Wall>(m, "Wall")
        .def(py::init<const ushort>())
        .def(py::init<const Rules&>())
        .def(py::init<const Wall&>())

        .def("clear", &Wall::clear)
        .def("is_placed_at", &Wall::is_placed_at)
        .def("get_placed", &Wall::get_placed);
}