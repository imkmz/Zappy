/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** ICamera
*/

#ifndef ICAMERA_HPP_
    #define ICAMERA_HPP_

#include "Raylib/raylib-cpp.hpp"

namespace Zappy {
    class ICamera {
        public:
            virtual ~ICamera() = default;

            /// @brief Camera update loop
            virtual void update() = 0;
            /// @brief Camera3D.BeginMode() wrapper
            virtual void beginMode() = 0;
            /// @brief Camera3D.EndMode() wrapper
            virtual void endMode() = 0;

            virtual raylib::Camera3D getCamera() const = 0;
    };
}

#endif /* !ICAMERA_HPP_ */
