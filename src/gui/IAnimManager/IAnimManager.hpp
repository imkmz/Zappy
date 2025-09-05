/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** IAnimManager
*/

#ifndef IANIMMANAGER_HPP_
    #define IANIMMANAGER_HPP_

#include "Raylib/raylib-cpp.hpp"

#include <memory>

namespace Zappy {

    enum State {
        IDLE,
        WALKING,
        EJECTING,
        INCANTING,
        EGG_LAYING,
        DROPPING,
        COLLECTING,
        DYING
    };

    class IAnimManager {
        public:
            ~IAnimManager() = default;

            virtual void setState(const State &s) = 0;
            virtual State getState() const = 0;
            virtual std::shared_ptr<raylib::Model> getModel() const = 0;
            virtual void updateAnim() = 0;
    };
}

#endif /* !IANIMMANAGER_HPP_ */
