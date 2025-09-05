#include "AnimManager.hpp"
#include <iostream>
#include <filesystem>

Zappy::AnimManager::AnimManager(const std::string &mdl)
{
    _mdl = std::make_shared<raylib::Model>(mdl);
    _anim = raylib::ModelAnimation::Load(mdl);
    _state = IDLE;
    _curFrame = 0;

    _stateArr = {
        {IDLE, 4},
        {WALKING, 8},
        {EJECTING, 6},
        {INCANTING, 5},
        {EGG_LAYING, 3},
        {DROPPING, 1},
        {COLLECTING, 0},
        {DYING, 2}
    };
}

void Zappy::AnimManager::setState(const State &s)
{
    _state = s;
    _curFrame = 0;
}

Zappy::State Zappy::AnimManager::getState() const
{
    return _state;
}

std::shared_ptr<raylib::Model> Zappy::AnimManager::getModel() const
{
    return _mdl;
}

void Zappy::AnimManager::updateAnim()
{
    if (_anim.size() <= 0 || !_anim[0].IsValid(*_mdl))
        return;

    auto i = _stateArr.find(_state);
    if (i == _stateArr.end() || (size_t)i->second >= _anim.size() || !_mdl)
        return;

    UpdateModelAnimation(*_mdl, _anim[i->second], _curFrame);
    _curFrame = (_curFrame + 1) % _anim[i->second].GetFrameCount();
}
