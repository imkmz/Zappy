#include "TeamManager.hpp"

Zappy::TeamManager::TeamManager(std::shared_ptr<ICache> cache)
{
    _cache = cache;
    _teams.push_back(std::make_shared<Team>("Unknown", cache));
}

void Zappy::TeamManager::draw(std::shared_ptr<Zappy::ICamera> camera)
{
    auto now = std::chrono::high_resolution_clock::now();

    for (auto &team : _teams)
        team->draw(camera);
    for (auto &ply : getPlayers()) {
        if (ply->isDying() && now - ply->getDeathTime() >= std::chrono::milliseconds(1500)) {
            getTeam(ply->getTeam())->removePlayer(ply->getId());
        }
    }
}

void Zappy::TeamManager::registerTeam(const std::string &team)
{
    _teams.push_back(std::make_shared<Team>(team, _cache));
}

std::shared_ptr<Zappy::Team> Zappy::TeamManager::getDefaultTeam() const
{
    return _teams[0];
}

std::shared_ptr<Zappy::Team> Zappy::TeamManager::getTeam(const std::string &team) const
{
    for (auto &t : _teams)
        if (t->getName() == team)
            return t;
    return nullptr;
}

std::shared_ptr<Zappy::Player> Zappy::TeamManager::getPlayer(int id) const
{
    for (auto &team : _teams) {
        auto ply = team->getPlayer(id);

        if (ply)
            return ply;
    }
    return nullptr;
}

std::shared_ptr<Zappy::Egg> Zappy::TeamManager::getEgg(int id) const
{
    for (auto &team : _teams) {
        auto egg = team->getEgg(id);

        if (egg)
            return egg;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Zappy::Player>> Zappy::TeamManager::getPlayersAtPos(raylib::Vector2 pos) const
{
    std::vector<std::shared_ptr<Zappy::Player>> players;

    for (auto &team : _teams)
        for (auto &ply : team->getPlayers())
            if (ply->getPos() == raylib::Vector3(pos.x, 0.5, pos.y))
                players.push_back(ply);
    return players;
}

void Zappy::TeamManager::removePlayer(int id)
{
    getTeam(getPlayer(id)->getTeam())->removePlayer(id);
}

void Zappy::TeamManager::removeEgg(int id)
{
    if (!getEgg(id))
        return;
    getTeam(getEgg(id)->getTeam())->removeEgg(id);
}

std::vector<std::shared_ptr<Zappy::Player>> Zappy::TeamManager::getPlayers() const
{
    std::vector<std::shared_ptr<Zappy::Player>> players;

    for (auto &team : _teams)
        for (auto &ply : team->getPlayers())
            players.push_back(ply);
    return players;
}

std::vector<std::shared_ptr<Zappy::Egg>> Zappy::TeamManager::getEggs() const
{
    std::vector<std::shared_ptr<Zappy::Egg>> eggs;

    for (auto &team : _teams)
        for (auto &egg : team->getEggs())
            eggs.push_back(egg);
    return eggs;
}

std::vector<std::shared_ptr<Zappy::Team>> Zappy::TeamManager::getTeams() const
{
    return _teams;
}
