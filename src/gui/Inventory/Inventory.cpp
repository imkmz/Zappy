/*
** EPITECH PROJECT, 2025
** inventory.cpp
** File description:
** inventory.cpp
*/

#include "Inventory.hpp"

Zappy::Inventory::Inventory()
{
    _inventory[FOOD] = 0;
    _inventory[LINEMATE] = 0;
    _inventory[DERAUMERE] = 0;
    _inventory[SIBUR] = 0;
    _inventory[MENDIANE] = 0;
    _inventory[PHIRAS] = 0;
    _inventory[THYSTAME] = 0;
}

void Zappy::Inventory::addItem(const Resources &res)
{
    _inventory[res] += 1;
}

void Zappy::Inventory::removeItem(const Resources &res)
{
    if (_inventory[res] > 0)
        _inventory[res] -= 1;
}

std::unordered_map<Zappy::Resources, int> Zappy::Inventory::getInventory() const
{
    return _inventory;
}

void Zappy::Inventory::setInventory(std::unordered_map<Resources, int> inventory)
{
    _inventory = inventory;
}

std::vector<std::string> Zappy::Inventory::getContent() const
{
    std::vector<std::string> content;

    for (const auto& item : _inventory) {
        std::string itemName;

        switch (item.first) {
            case FOOD: itemName = "Burger"; break;
            case LINEMATE: itemName = "Milk"; break;
            case DERAUMERE: itemName = "Ball"; break;
            case SIBUR: itemName = "Cardboard"; break;
            case MENDIANE: itemName = "Feather"; break;
            case PHIRAS: itemName = "Fish"; break;
            case THYSTAME: itemName = "Mouse"; break;
            default: itemName = "UNKNOWN"; break;
        }
        content.push_back(itemName + ": " + std::to_string(item.second));
    }
    return content;
}
