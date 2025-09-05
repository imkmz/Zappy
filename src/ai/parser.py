##
## EPITECH PROJECT, 2025
## zappy
## File description:
## parser
##

def parse_inventory(response):
    response = response.strip("[]\n")
    items = response.split(",")
    inventory = {}
    for item in items:
        parts = item.strip().split(" ")
        if len(parts) == 2:
            name, count = parts
            inventory[name] = int(count)
    return inventory

def parse_look(response):
    response = response.strip("[]\n")
    tiles = [tile.strip() for tile in response.split(",")]
    parsed = []
    for tile in tiles:
        if tile:
            parsed.append(tile.split(" "))
        else:
            parsed.append([])
    return parsed
