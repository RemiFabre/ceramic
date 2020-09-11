#ifndef GAME_HPP
#define GAME_HPP

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <limits>
#include <memory>
#include <vector>

#include "action.hpp"
#include "rules/rules.hpp"
#include "state/state.hpp"

using namespace boost::random;

typedef mt11213b rng;
typedef uniform_int_distribution<> int_range;

class Game {

private:
    State state;
    rng randomness;
    int_range range;

    int rand(int min = 0, int max = std::numeric_limits<int>::max());
    Tile pull_one_random_tile();
    Tiles pull_random_tiles(int count);

public:
    Game();
    Game(const std::shared_ptr<Rules>& rules);

    void start();
    void start_round();
    void end_round();

    void setup_factories();
    void score_panels();
    void static score_panels(State& state);
    void use_first_token();
    void static use_first_token(State& state);

    bool legal(Action action) const;
    void apply(Action action);

    bool static legal(Action action, const State& state);
    void static apply(Action action, State& state);
};

#endif //GAME_HPP
