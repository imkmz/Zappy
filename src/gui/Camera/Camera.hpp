/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Camera
*/

#ifndef CAMERA_HPP_
    #define CAMERA_HPP_

#include <unordered_map>
#include <functional>

#include "ICamera/ICamera.hpp"

namespace Zappy {
    class Camera : public ICamera {
        public:
            Camera();
            ~Camera() = default;

            /// @brief Camera update loop
            void update() override;
            /// @brief Camera3D.BeginMode() wrapper
            void beginMode() override;
            /// @brief Camera3D.EndMode() wrapper
            void endMode() override;

            raylib::Camera3D getCamera() const;
        private:
            raylib::Camera3D _camera;
            raylib::Vector3 _direction;
            raylib::Vector3 _front;
            raylib::Vector3 _right;
            raylib::Vector3 _up;
            float _pitch;
            float _yaw;
    };
}

#endif /* !CAMERA_HPP_ */
