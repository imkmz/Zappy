/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** ICache
*/

#ifndef ICACHE_HPP_
    #define ICACHE_HPP_

#include <memory>
#include "Raylib/raylib-cpp.hpp"
#include "Resources/Resources.hpp"

namespace Zappy {
    enum ModelId {
        M_FOOD,
        M_LINEMATE,
        M_DERAUMERE,
        M_SIBUR,
        M_MENDIANE,
        M_PHIRAS,
        M_THYSTAME,
        M_PLAYER,
        M_TILE,
        M_EGG
    };

    enum ColorId {
        COL_MAIN,
        COL_ACCENT,
        COL_SKY
    };

    class ICache {
    public:
        virtual ~ICache() = default;

        virtual raylib::Vector3 getOffset(const ModelId &id) const = 0;
        virtual raylib::Vector3 getOffset(const Resources &id) const = 0;
        virtual double getRotation(const ModelId &id) const = 0;
        virtual double getRotation(const Resources &id) const = 0;
        virtual std::shared_ptr<raylib::Texture2D> getTexture(const ModelId &id) const = 0;
        virtual std::shared_ptr<raylib::Texture2D> getTexture(const Resources &id) const = 0;
        virtual std::shared_ptr<raylib::Color> getColor(const ColorId &id) const = 0;
        virtual std::shared_ptr<raylib::Model> getModel(const ModelId &id) const = 0;
        virtual std::shared_ptr<raylib::Model> getModel(const Resources &res) const = 0;
        virtual double getScale(const ModelId &id) const = 0;
        virtual double getScale(const Resources &res) const = 0;
    };
}

#endif // ICACHE_HPP_