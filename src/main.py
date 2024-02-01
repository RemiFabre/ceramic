import random

from ceramic.game import Action, Game, GameHelper, Player
from ceramic.players import MonteCarloPlayer, RandomPlayer
from ceramic.rules import Rules
from ceramic.state import Tile


class TestPlayer(Player):
    def __init__(self):
        Player.__init__(self)

    def play(self, state):
        special_action = Action(1, Tile.from_letter("B"), 3)
        if GameHelper.legal(special_action, state):
            return special_action
        legal_actions = GameHelper.all_legal(state)
        return random.choice(legal_actions)


rules = Rules.BASE
rules.player_count = 2
game = Game(rules)
test_player = TestPlayer()  # <-- here
game.add_player(MonteCarloPlayer())
game.add_player(MonteCarloPlayer())
while not game.state.is_game_finished():
    game.start_round()
    round_tree_size = 1
    while not game.state.is_round_finished():
        legal_actions = GameHelper.all_legal(game.state)
        nb_legal = len(legal_actions)
        print(f"number of legal actions: {nb_legal}")
        round_tree_size *= nb_legal

        action = test_player.play(game.state)
        game.apply(action)
    game.end_round()
    print(f"Round tree size: {round_tree_size}")
    # print(f"Action played: {action}")
    # print(f"Game state: {game.state}")

# game.roll_game()  # Plays a random game until the end
# print("The winner is:", game.state.winning_player())
# print(f"Game state: {game.state}")
