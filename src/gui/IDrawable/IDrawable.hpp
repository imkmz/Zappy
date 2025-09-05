/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** IDrawable
*/

#ifndef IDRAWABLE_HPP_
    #define IDRAWABLE_HPP_

#include "Camera/Camera.hpp"
#include <memory>

namespace Zappy {
    class IDrawable {
        public:
            ~IDrawable() = default;

            virtual void draw(std::shared_ptr<Zappy::ICamera> camera) const = 0;
            virtual void update() = 0;
    };
}

#endif /* !IDRAWABLE_HPP_ */
