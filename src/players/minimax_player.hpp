#ifndef MINIMAX_PLAYER_HPP
#define MINIMAX_PLAYER_HPP

#include "game/action.hpp"
#include "game/player.hpp"
#include "state/state.hpp"
#include <limits>
#include <memory>
#include <vector>

class MinimaxPlayer : public Player {
private:
    int depth; // The depth of the search in the Minimax algorithm.

    // Minimax algorithm with Alpha-Beta pruning.
    float minimax(Game& game, State state, int depth, int player, float alpha, float beta, bool maximizingPlayer);

    // Placeholder evaluation function returning a random value.
    float evaluate_state(const State& state, int player) const;

public:
    explicit MinimaxPlayer(int depth = 3); // Constructor with an optional depth parameter.

    // Returns a shared pointer to a new player object copied from this one.
    virtual std::shared_ptr<Player> copy() const override;

    // Determines and returns the best action to take from a given game state.
    virtual Action play(const State& state) override;

    // Returns a string describing the player type, including the search depth.
    virtual std::string player_type() const override;
};

#endif // MINIMAX_PLAYER_HPP
