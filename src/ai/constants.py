#!/usr/bin/env python3
##
## EPITECH PROJECT, 2025
## zappy
## File description:
## constants
##


RESOURCE_TYPES = [
    "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
]

ELEVATION_REQUIREMENTS = {
    1: {'players': 1, 'linemate': 1},
    2: {'players': 2, 'linemate': 1, 'deraumere': 1, 'sibur': 1},
}

def format_broadcast_message(msg_type, data: dict) -> str:
    parts = [f"{key}={value}" for key, value in data.items()]
    return f"{msg_type} " + " ".join(parts)