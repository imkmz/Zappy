#include "Team.hpp"

Zappy::Team::Team(const std::string &name, std::shared_ptr<ICache> cache) :
    ADrawable(cache, {})
{
    _name = name;
    _cache = cache;
}

void Zappy::Team::draw(std::shared_ptr<Zappy::ICamera> camera) const
{
    for (auto &ply : _players)
        ply->draw(camera);
    for (auto &egg : _eggs)
        egg->draw(camera);
}

void Zappy::Team::update()
{
    return;
}

std::string Zappy::Team::getName() const
{
    return _name;
}

std::vector<std::shared_ptr<Zappy::Player>> Zappy::Team::getPlayers() const
{
    return _players;
}

std::vector<std::shared_ptr<Zappy::Egg>> Zappy::Team::getEggs() const
{
    return _eggs;
}

void Zappy::Team::addPlayer(int id, int x, int y, int orientation, int level)
{
    _players.push_back(std::make_shared<Player>(id, x, y, orientation, level, _name, _cache));
}

void Zappy::Team::addPlayer(int x, int y, int orientation, int level)
{
    std::shared_ptr<Player> newPlayer = std::make_shared<Player>(_players.size(), x, y, orientation, level, _name, _cache);

    _players.push_back(newPlayer);
}

void Zappy::Team::removePlayer(int id)
{
    auto ply = getPlayer(id);

    if (ply)
        _players.erase(std::find(_players.begin(), _players.end(), ply));
}

std::shared_ptr<Zappy::Player> Zappy::Team::getPlayer(int id) const
{
    for (auto &ply : _players)
        if (ply->getId() == id)
            return ply;
    return nullptr;
}

void Zappy::Team::addEgg(int id, int x, int y)
{
    auto egg = std::make_shared<Egg>(id, static_cast<float>(x), static_cast<float>(y), _name, _cache);

    _eggs.push_back(egg);
}

void Zappy::Team::removeEgg(int id)
{
    auto egg = getEgg(id);

    if (egg)
        _eggs.erase(std::find(_eggs.begin(), _eggs.end(), egg));
}

std::shared_ptr<Zappy::Egg> Zappy::Team::getEgg(int id) const
{
    for (auto &egg : _eggs)
        if (egg->getId() == id)
            return egg;
    return nullptr;
}
