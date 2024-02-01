#include "minimax_player.hpp"
#include "game/game.hpp"
#include <algorithm>
#include <iostream>
#include <random>

MinimaxPlayer::MinimaxPlayer(int depth) : depth(depth) {}

std::shared_ptr<Player> MinimaxPlayer::copy() const {
    return std::make_shared<MinimaxPlayer>(*this);
}

float MinimaxPlayer::evaluate_state(const State& state, int player) const {
    // Placeholder: return a random float between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    return dis(gen);
}

float MinimaxPlayer::minimax(Game& game, State state, int depth, int player, float alpha, float beta, bool maximizingPlayer) {
    if (depth == 0 || state.is_game_finished()) {
        return evaluate_state(state, player);
    }

    std::vector<Action> legal_actions = Game::all_legal(state);

    if (maximizingPlayer) {
        float maxEval = -std::numeric_limits<float>::infinity();
        for (const Action& action : legal_actions) {
            State new_state(state);
            Game::apply(action, new_state);
            float eval = minimax(game, new_state, depth - 1, player, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break; // Beta cut-off
        }
        return maxEval;
    } else {
        float minEval = std::numeric_limits<float>::infinity();
        for (const Action& action : legal_actions) {
            State new_state(state);
            Game::apply(action, new_state);
            float eval = minimax(game, new_state, depth - 1, player, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break; // Alpha cut-off
        }
        return minEval;
    }
}

Action MinimaxPlayer::play(const State& state) {
    int player = state.get_current_player();
    Game game = Game(state.get_rules());

    std::vector<Action> legal_actions = Game::all_legal(state);
    std::shuffle(legal_actions.begin(), legal_actions.end(), std::mt19937(std::random_device()()));

    float best_value = -std::numeric_limits<float>::infinity();
    Action best_action;

    for (const Action& action : legal_actions) {
        State new_state(state);
        Game::apply(action, new_state);
        float value = minimax(game, new_state, depth - 1, player, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), false);
        if (value > best_value) {
            best_value = value;
            best_action = action;
        }
    }

    return best_action;
}

std::string MinimaxPlayer::player_type() const {
    return "Minimax-" + std::to_string(depth);
}
