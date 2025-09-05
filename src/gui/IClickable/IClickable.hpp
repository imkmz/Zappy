/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** IClickable
*/

#ifndef ICLICKABLE_HPP_
    #define ICLICKABLE_HPP_

#include "Raylib/raylib-cpp.hpp"

namespace Zappy {
    class IClickable {
        public:
            IClickable() = default;
            ~IClickable() = default;

            virtual void setPos(const raylib::Vector3 &pos) = 0;
            /// @brief Gets the tooltip content for a clickable object
            /// @return THe lines that should be drawn in order in the tooltip
            virtual std::vector<std::string> getToolTip() const = 0;
            /// @brief Returns wether a clickable object is clicked or not
            /// @param ray The ray shot from the mouse
            /// @return Is the object clicked
            virtual bool isClicked(const raylib::Ray &ray) const = 0;
            virtual raylib::RayCollision getCollision(const raylib::Ray &ray) const = 0;
    };
}

#endif /* !ICLICKABLE_HPP_ */
