#include <iostream>

#include <memory>
#include <unistd.h>

#include "analysis/all_arena.hpp"
#include "analysis/arena.hpp"
#include "analysis/pairs_arena.hpp"
#include "players/first_legal_player.hpp"
#include "players/monte_carlo_player.hpp"
#include "players/random_player.hpp"

void
print_help_players() {
    std::cout << "        fl : First Legal Player\n";
    std::cout << "        rn : Naive Random Player (smart = false)\n";
    std::cout << "        r : Random Player\n";
    std::cout << "        mc / mc:<rollouts> : Monte-Carlo Player (rollouts default is 1000)\n";
}

void
print_help() {
    std::cout << "./ceramic-arena [-h] <players> [-a <arena_type>] [-g <game_per_group>] [-t <thread_limit>]\n";
    std::cout << '\n';
    std::cout << "    -h : Help, shows this]\n";
    std::cout << '\n';
    std::cout << "    <players> : list for each player (default is 'fl rn r')\n";
    print_help_players();
    std::cout << '\n';
    std::cout << "    -a <arena_type>\n";
    std::cout << "        s : subsets\n";
    std::cout << "        p : pairs (default)\n";
    std::cout << "        a : all\n";
    std::cout << '\n';
    std::cout << "    -g <game_per_group> : int (default is 1000)\n";
    std::cout << "    -t <thread_limit> : int (default is 8)\n";
    std::cout << std::endl;
}

struct ArenaOptions {
    int count;
    int thread_limit;
};

enum ArenaMode {
    SUBSETS,
    PAIRS,
    ALL,
};

bool
options(int argc, char* argv[], vector<std::shared_ptr<Player>>& players, std::shared_ptr<Rules>& rules, ArenaMode& arena_mode, ArenaOptions& arena_options) {
    int option;
    while ((option = getopt(argc, argv, ":hc:n:a:g:t:")) != -1) { //get option from the getopt() method
        switch (option) {
            // Help
            case 'h':
                print_help();
                return false;
            // Arena
            case 'a':
                switch (optarg[0]) {
                    case 's':
                        arena_mode = ArenaMode::SUBSETS;
                        break;
                    case 'p':
                        arena_mode = ArenaMode::PAIRS;
                        break;
                    case 'a':
                        arena_mode = ArenaMode::ALL;
                        break;
                    default:
                        std::cout << "Unrecognised arena argument: " << optarg << '\n';
                        std::cout << "Use 's' for subsets, 'p' pairs, 'a' for all" << std::endl;
                        return false;
                }
                break;
            case 'g':
                arena_options.count = std::stoi(optarg);
                break;
            case 't':
                arena_options.thread_limit = std::stoi(optarg);
                break;
            // Rules
            case 'n':
                try {
                    int value = std::stoi(optarg);
                    if (value < 2 || value > TILE_TYPES) {
                        throw out_of_range("");
                    }
                    rules->tile_types = value;
                } catch (const exception& e) {
                    std::cout << "Unrecognised tile types cout: " << optarg << '\n';
                    std::cout << "Use an int between 2 and " << TILE_TYPES << " (included)" << std::endl;
                    return false;
                }
                break;
            // Errors
            case ':':
                std::cout << "Missing argument for option -" << char(optopt) << '\n';
                print_help();
                return false;
            case '?':
                std::cout << "Unkown option -" << char(optopt) << '\n';
                print_help();
                return false;
        }
    }
    for (; optind < argc; optind++) {
        string arg = argv[optind];
        if (arg == "fl" || arg == "first-legal") {
            players.push_back(std::make_shared<FirstLegalPlayer>());
        } else if (arg == "rn" || arg == "rand-naive" || arg == "random-naive") {
            players.push_back(std::make_shared<RandomPlayer>(false));
        } else if (arg == "r" || arg == "rand" || arg == "random") {
            players.push_back(std::make_shared<RandomPlayer>());
        } else if (arg == "mc" || arg == "monte-carlo") {
            players.push_back(std::make_shared<MonteCarloPlayer>());
        } else {
            std::cout << "Unkown player type " << arg << '\n';
            print_help();
            return false;
        }
    }
    if (players.size() == 0) {
        players = {
            std::make_shared<FirstLegalPlayer>(),
            std::make_shared<RandomPlayer>(false),
            std::make_shared<RandomPlayer>()
        };
    }
    return true;
}

int
main(int argc, char* argv[]) {
    vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Rules> rules = std::make_shared<Rules>(*Rules::DEFAULT);
    ArenaMode arena_mode = ArenaMode::PAIRS;
    ArenaOptions arena_options{ count : 1000, thread_limit : 8 };
    if (!options(argc, argv, players, rules, arena_mode, arena_options)) {
        return 1;
    }
    std::unique_ptr<Arena> arena;
    switch (arena_mode) {
        case ArenaMode::SUBSETS:
            arena = std::make_unique<Arena>(rules, players);
            break;
        case ArenaMode::PAIRS:
            arena = std::make_unique<PairsArena>(rules, players);
            break;
        case ArenaMode::ALL:
            arena = std::make_unique<AllArena>(rules, players);
            break;
        default:
            throw runtime_error("Unkown Arena Mode");
    }
    arena->count = arena_options.count;
    arena->thread_limit = arena_options.thread_limit;
    arena->run();
}