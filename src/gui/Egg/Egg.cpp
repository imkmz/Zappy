/*
** EPITECH PROJECT, 2025
** Egg.cpp
** File description:
** Egg.cpp
*/

#include "Egg.hpp"

Zappy::Egg::Egg(int id, float x, float y, std::string team, std::shared_ptr<ICache> cache) :
    AClickable(M_EGG, cache, {x, 0.5, y}),
    ADrawable(cache, {x, 0.5, y})
{
    _id = id;
    _team = team;
}

void Zappy::Egg::draw([[maybe_unused]] std::shared_ptr<Zappy::ICamera> camera) const
{
    auto model = _cache->getModel(M_EGG);

    if (model && model->IsValid())
        model->Draw(_pos, _cache->getScale(M_EGG));
}

void Zappy::Egg::update()
{
    return;
}

std::string Zappy::Egg::getTeam() const
{
    return _team;
}

int Zappy::Egg::getId() const
{
    return _id;
}

std::vector<std::string> Zappy::Egg::getToolTip() const
{
    std::stringstream eggId;
    std::stringstream teamName;
    std::string tn = (_team == "") ? "Unknown Team" : _team;

    eggId << "Egg: " << _id;
    teamName << "Team: " << tn;
    return {
        eggId.str(),
        teamName.str()
    };
}
