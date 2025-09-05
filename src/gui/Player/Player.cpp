/*
** EPITECH PROJECT, 2025
** player.cpp
** File description:
** player.cpp
*/

#include "Player.hpp"
#include <memory>


Zappy::Player::Player(int id, int x, int y, int orientation, int level, std::string team, std::shared_ptr<ICache> cache) :
    AClickable(M_PLAYER, cache, {static_cast<float>(x), 0.5, static_cast<float>(y)}),
    ADrawable(cache, {static_cast<float>(x), 0.5, static_cast<float>(y)})
{
    _id = id;
    _orientation = orientation;
    _level = level;
    _team = team;
    _inventory = std::make_shared<Inventory>();
    _isInIncantation = false;
    _animManager = std::make_shared<AnimManager>("assets/models/cat.glb");
}

void Zappy::Player::draw(std::shared_ptr<Zappy::ICamera> cam) const
{
    auto model = _animManager->getModel();

    static const std::unordered_map<int, float> orMap = {
        {1, 3.1f}, {2, 1.55f}, {3, 0.0f}, {4, 4.65f}
    };

    for (auto &[orient, rot] : orMap)
        if (orient == _orientation)
            model->transform = raylib::Matrix::RotateY(rot);
    if (model && model->IsValid())
        model->Draw(_pos, _cache->getScale(M_PLAYER));

    if (!_message.empty()) {
        raylib::Vector3 textPos {_pos.x, _pos.y + 0.8f, _pos.z};
        Vector2 screenPos = GetWorldToScreen(textPos, cam->getCamera());

        Vector3 camPos = cam->getCamera().position;

        float dist = sqrtf(
            (camPos.x - textPos.x) * (camPos.x - textPos.x) +
            (camPos.y - textPos.y) * (camPos.y - textPos.y) +
            (camPos.z - textPos.z) * (camPos.z - textPos.z)
        );

        int fontSize = std::clamp(int(200.0f / (dist + 1.0f)), 10, 50);
        int textWidth = MeasureText(_message.c_str(), fontSize);

        cam->endMode();
            DrawText(_message.c_str(), screenPos.x - textWidth / 2, screenPos.y, fontSize, BLACK);
        cam->beginMode();
    }
}

void Zappy::Player::update()
{
    auto now = std::chrono::high_resolution_clock::now();

    if (now - _lastSpeach > std::chrono::milliseconds(4000))
        _message = "";
    _animManager->updateAnim();
}

void Zappy::Player::setState(const State &s)
{
    _animManager->setState(s);
}

std::vector<std::string> Zappy::Player::getToolTip() const
{
    std::stringstream plyId;
    std::stringstream plyLvl;
    std::stringstream plyHealth;


    plyId << "Player: " << _id;
    plyLvl << "Level: " << _level;
    plyHealth << "Health: " << _health;

    std::vector<std::string> tt;
    tt.push_back(plyId.str());
    tt.push_back(plyLvl.str());
    tt.push_back(plyHealth.str());
    tt.push_back("");
    tt.push_back("Inventory:");
    tt.push_back("");
    for (auto &item : _inventory->getContent())
        tt.push_back(item);
    return tt;
}

std::shared_ptr<Zappy::Inventory> Zappy::Player::getInventory() const
{
    return _inventory;
}

int Zappy::Player::getLevel() const
{
    return _level;
}

int Zappy::Player::getHealth() const
{
    return _health;
}

void Zappy::Player::setInventory(std::unordered_map<Resources, int> inventory)
{
    _inventory->setInventory(inventory);
}

void Zappy::Player::setPosition(int x, int y)
{
    _pos.x = static_cast<float>(x);
    _pos.z = static_cast<float>(y);
    AClickable::setPos(raylib::Vector3(static_cast<float>(x), 0.5f, static_cast<float>(y)));
}

void Zappy::Player::setDying()
{
    _isDying = true;
    _deathTime = std::chrono::high_resolution_clock::now();
    if (_animManager)
        _animManager->setState(DYING);
}

std::chrono::high_resolution_clock::time_point Zappy::Player::getDeathTime() const
{
    return _deathTime;
}

bool Zappy::Player::isDying() const
{
    return _isDying;
}

void Zappy::Player::setLevel(int level)
{
    _level = level;
}

void Zappy::Player::setOrientation(int orientation)
{
    _orientation = orientation;
}

void Zappy::Player::speak(const std::string &msg)
{
    _message = msg;
    _lastSpeach = std::chrono::high_resolution_clock::now();
}

int Zappy::Player::getId() const
{
    return _id;
}

std::string Zappy::Player::getTeam() const
{
    return _team;
}

void Zappy::Player::startIncantation(int x, int y)
{
    _isInIncantation = true;
    _pos.x = x;
    _pos.z = y;
}

void Zappy::Player::endIncantation([[maybe_unused]] bool result)
{
    _isInIncantation = false;
    _animManager->setState(IDLE);
}


