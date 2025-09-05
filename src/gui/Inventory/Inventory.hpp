/*
** EPITECH PROJECT, 2025
** inventory.hpp
** File description:
** inventory.hpp
*/

#ifndef INVENTORY_HPP
    #define INVENTORY_HPP

#include "Resources/Resources.hpp"

#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace Zappy
{

    class Inventory
    {
    public:
        Inventory();
        ~Inventory() = default;

        void addItem(const Resources &res);
        void removeItem(const Resources &res);

        std::unordered_map<Resources, int> getInventory() const;
        void setInventory(std::unordered_map<Resources, int> inventory);
        std::vector<std::string> getContent() const;

    private:
        std::unordered_map<Resources, int> _inventory;
    };
}
#endif // INVENTORY_HPP