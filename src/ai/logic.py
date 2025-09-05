##
## EPITECH PROJECT, 2025
## zappy
## File description:
## logic
##

import random
from parser import parse_look, parse_inventory
from constants import format_broadcast_message

ELEVATION_REQUIREMENTS = {
    1: {'players': 1, 'linemate': 1},
    2: {'players': 2, 'linemate': 1, 'deraumere': 1, 'sibur': 1},
    3: {'players': 2, 'linemate': 2, 'sibur': 1, 'phiras': 2},
    4: {'players': 4, 'linemate': 1, 'deraumere': 1, 'sibur': 2, 'phiras': 1},
    5: {'players': 4, 'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
    6: {'players': 6, 'linemate': 1, 'deraumere': 2, 'sibur': 3, 'phiras': 1},
    7: {'players': 6, 'linemate': 2, 'deraumere': 2, 'sibur': 2, 'mendiane': 2, 'phiras': 2, 'thystame': 1}
}

class AILogic:
    def __init__(self):
        self.level = 1
        self.inventory = {
            'food': 10,
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }
        self.position = {'x': 0, 'y': 0}
        self.orientation = 1
        self.command_queue = []
        self.vision = []

    def find_resource_in_vision(self, resource):
        for i, tile in enumerate(self.vision):
            if resource in tile:
                return i
        return None

    def random_direction(self):
        if random.random() < 0.3:
            self.command_queue.append("Right" if random.random() < 0.5 else "Left")
        else:
            self.command_queue.append("Forward")

    def direction_to_command(self, direction):
        if direction in [1, 2, 8]:
            return "Forward"
        elif direction in [3, 4]:
            return "Right"
        elif direction in [5, 6]:
            return "Left"
        else:
            return "Forward"

    def count_players_on_tile(self):
        return self.vision[0].count("player") if self.vision else 0

    def get_elevation_requirements(self):
        return ELEVATION_REQUIREMENTS.get(self.level, {})

    def can_elevate(self):
        reqs = self.get_elevation_requirements()
        for res, needed in reqs.items():
            if res == 'players':
                continue
            if self.inventory.get(res, 0) < needed:
                return False
        return True

    def missing_resources(self):
        reqs = self.get_elevation_requirements()
        missing = []
        for res, qty in reqs.items():
            if res == 'players':
                continue
            if self.inventory.get(res, 0) < qty:
                missing.extend([res] * (qty - self.inventory.get(res, 0)))
        return missing

    def decide_next_action(self):
        if self.can_elevate():
            players_needed = self.get_elevation_requirements().get("players", 1)
            if self.count_players_on_tile() < players_needed:
                self.command_queue.append("Look")
                return
            for res, qty in self.get_elevation_requirements().items():
                if res == "players":
                    continue
                for _ in range(qty):
                    if self.inventory.get(res, 0) > 0:
                        self.command_queue.append(f"Set {res}")
                        self.inventory[res] -= 1
            self.command_queue.append("Incantation")
            return
        if self.find_resource_in_vision("food") == 0:
            self.command_queue.append("Take food")
            return
        needed = self.missing_resources()
        for res in needed:
            if self.find_resource_in_vision(res) == 0:
                self.command_queue.append(f"Take {res}")
                return
        self.random_direction()

    def update_from_look(self, look_result):
        self.vision = parse_look(look_result)

    def update_inventory(self, inv_response):
        self.inventory = parse_inventory(inv_response)
