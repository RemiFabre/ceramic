#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "action.hpp"
#include "game.hpp"
#include "player.hpp"
#include "py_utils.hpp"

namespace py = pybind11;

Action
py_create_action(
    ushort pick,
    Tile color,
    ushort place) {
    return Action{
        .pick = pick,
        .color = color,
        .place = place,
    };
}

class PyPlayer : public Player {
public:
    using Player::Player;

    bool check_rules(const Rules& rules) const override {
        PYBIND11_OVERLOAD(
            bool,
            Player,
            check_rules,
            rules);
    }
    std::shared_ptr<Observer> observer() override {
        PYBIND11_OVERLOAD(
            std::shared_ptr<Observer>,
            Player,
            observer);
    }
    Action play(const State& state) override {
        PYBIND11_OVERLOAD_PURE(
            Action,
            Player,
            play,
            state);
    }
    void error(string message) override {
        PYBIND11_OVERLOAD(
            void,
            Player,
            error,
            message);
    }
    string player_type() const override {
        PYBIND11_OVERLOAD(
            string,
            Player,
            player_type);
    }
};

class PyObserver : public Observer {
public:
    using Observer::Observer;

    void start_game(vector<ushort> order) override {
        PYBIND11_OVERLOAD(
            void,
            Observer,
            start_game,
            order);
    }
    void new_round(const State& state) override {
        PYBIND11_OVERLOAD(
            void,
            Observer,
            new_round,
            state);
    }
    void action_played(Action action) override {
        PYBIND11_OVERLOAD(
            void,
            Observer,
            action_played,
            action);
    }
    void end_game(const State& state, ushort winner_position) override {
        PYBIND11_OVERLOAD(
            void,
            Observer,
            end_game,
            state,
            winner_position);
    }
};

void
py_bind_game(py::module& root) {
    py::module m = root.def_submodule("game");

    py::class_<Action>(m, "Action")
        .def(py::init(&py_create_action),
            py::arg("pick"),
            py::arg("color"),
            py::arg("place"))

        .def_readwrite("pick", &Action::pick)
        .def_readwrite("color", &Action::color)
        .def_readwrite("place", &Action::place)

        .def("__eq__", &py_eq<Action>)
        .def("__ne__", &py_ne<Action>)

        .def("__str__", &Action::str)
        .def("__repr__", &Action::repr);

    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def(py::init<std::shared_ptr<Rules>>())
        .def(py::init<std::shared_ptr<Rules>, int>())
        .def(py::init<std::shared_ptr<Rules>, vector<std::shared_ptr<Player>>>())

        .def_property_readonly("state", &Game::get_state)

        .def("players_missing", &Game::players_missing)
        .def("has_enough_players", &Game::has_enough_players)
        .def("add_player", &Game::add_player)
        .def("add_players", &Game::add_players)

        .def("reset", &Game::reset)
        .def("start_round", &Game::start_round)
        .def("end_round", &Game::end_round)
        .def("score_final", &Game::score_final)

        .def("roll_round", &Game::roll_round)
        .def("roll_game", &Game::roll_game)
        .def("next_player", &Game::next_player)

        .def("setup_factories", &Game::setup_factories)
        .def("score_panels", [](Game& game) { game.score_panels(); })
        .def("apply_first_token", [](Game& game) { game.apply_first_token(); })

        .def("legal", [](const Game& game, Action action) { return game.legal(action); })
        .def("apply", [](Game& game, Action action) { game.apply(action); });

    py::class_<Player, std::shared_ptr<Player>, PyPlayer>(m, "Player")
        .def(py::init<>())

        .def("check_rules", &Player::check_rules)

        .def("observer", &Player::observer)

        .def("play", &Player::play)
        .def("error", &Player::error)

        .def("player_type", &Player::player_type)
        .def("__str__", &Player::str)
        .def("__repr__", &Player::repr);

    py::class_<Observer, std::shared_ptr<Observer>, PyObserver>(m, "Observer")
        .def(py::init<>())

        .def("start_game", &Observer::start_game)

        .def("new_round", &Observer::new_round)

        .def("action_played", &Observer::action_played)
        .def("end_game", &Observer::end_game);

    m.def("legal", [](Action action, const State& state) { return Game::legal(action, state); });
    m.def("all_legal", &Game::all_legal);
    m.def("apply", [](Action action, State& state) { Game::apply(action, state); });
}