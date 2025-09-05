#include "Camera.hpp"

Zappy::Camera::Camera() :
    _camera({2.5, 2.5, 3}, {0, 0, 0}, {0, 1, 0}, 65, CAMERA_PERSPECTIVE)
{
    _pitch = -0.6;
    _yaw = -2.45;
    _direction.x = cos(_yaw) * cos(_pitch);
    _direction.y = sin(_pitch);
    _direction.z = sin(_yaw) * cos(_pitch);
    _front = Vector3Normalize(_direction);
    _right = Vector3Normalize(Vector3CrossProduct(_camera.up, _front));
    _up = Vector3CrossProduct(_direction, _right);
    _camera.target = Vector3Add(_camera.position, _front);
}

void Zappy::Camera::update()
{
    float deltaTime = GetFrameTime();

    const std::unordered_map<int, std::function<void()>> movementMap = {
        {KEY_W, [this, deltaTime]() {
            _camera.position = Vector3Add(_camera.position, Vector3Scale(_front, deltaTime * 2.0));
        }},
        {KEY_S, [this, deltaTime]() {
            _camera.position = Vector3Subtract(_camera.position, Vector3Scale(_front, deltaTime * 2.0));
        }},
        {KEY_A, [this, deltaTime]() {
            _camera.position = Vector3Subtract(_camera.position, Vector3Scale(Vector3CrossProduct(_front, _up), deltaTime * 2.0));
        }},
        {KEY_D, [this, deltaTime]() {
            _camera.position = Vector3Add(_camera.position, Vector3Scale(Vector3CrossProduct(_front, _up), deltaTime * 2.0));
        }},
        {KEY_SPACE, [this, deltaTime]() {
            _camera.position = Vector3Add(_camera.position, Vector3Scale(_up, deltaTime * 2.0));
        }},
        {KEY_LEFT_SHIFT, [this, deltaTime]() {
            _camera.position = Vector3Subtract(_camera.position, Vector3Scale(_up, deltaTime * 2.0));
        }},
        {KEY_LEFT, [this, deltaTime]() {
            _yaw -= deltaTime * 0.8;
        }},
        {KEY_RIGHT, [this, deltaTime]() {
            _yaw += deltaTime * 0.8;
        }},
        {KEY_UP, [this, deltaTime]() {
            _pitch += deltaTime * 0.8;
        }},
        {KEY_DOWN, [this, deltaTime]() {
            _pitch -= deltaTime * 0.8;
        }}
    };

    for (const auto &[key, action] : movementMap) {
        if (IsKeyDown(key))
            action();
    }

    if (raylib::Mouse::IsButtonDown(MOUSE_RIGHT_BUTTON)) {
        if (!IsCursorHidden())
            DisableCursor();
        Vector2 mouseDelta = GetMouseDelta();

        _yaw += mouseDelta.x * deltaTime * 0.3;
        _pitch += -mouseDelta.y * deltaTime * 0.3;
    } else
        if (IsCursorHidden())
            EnableCursor();

    _direction.x = cos(_yaw) * cos(_pitch);
    _direction.y = sin(_pitch);
    _direction.z = sin(_yaw) * cos(_pitch);
    _front = Vector3Normalize(_direction);
    _right = Vector3Normalize(Vector3CrossProduct(_camera.up, _front));
    _up = Vector3CrossProduct(_direction, _right);
    _camera.target = Vector3Add(_camera.position, _front);
}

void Zappy::Camera::beginMode()
{
    _camera.BeginMode();
}

void Zappy::Camera::endMode()
{
    _camera.EndMode();
}

raylib::Camera3D Zappy::Camera::getCamera() const
{
    return _camera;
}
