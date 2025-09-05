/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** TeamManager
*/

#ifndef TEAMMANAGER_HPP_
    #define TEAMMANAGER_HPP_

#include <vector>
#include <memory>

#include "Team/Team.hpp"

namespace Zappy {
    class TeamManager {
        public:
            TeamManager(std::shared_ptr<ICache> cache);
            ~TeamManager() = default;

            /**
             * @brief Draws all teams using the given camera.
             * @param camera The camera to use for rendering.
             */
            void draw(std::shared_ptr<Zappy::ICamera> camera);

            /**
             * @brief Registers a new team by name.
             * @param team The name of the team to register.
             */
            void registerTeam(const std::string &team);

            std::shared_ptr<Team> getDefaultTeam() const;

            std::vector<std::shared_ptr<Team>> getTeams() const;
            std::shared_ptr<Team> getTeam(const std::string &team) const;

            /**
             * @brief Removes a player from all teams by id.
             * @param id The id of the player to remove.
             */
            void removePlayer(int id);

            /**
             * @brief Removes an egg from all teams by id.
             * @param id The id of the egg to remove.
             */
            void removeEgg(int id);

            std::shared_ptr<Player> getPlayer(int id) const;
            std::shared_ptr<Egg> getEgg(int id) const;

            std::vector<std::shared_ptr<Player>> getPlayers() const;
            std::vector<std::shared_ptr<Egg>> getEggs() const;

            /**
             * @brief Gets all players at a specific position.
             * @param pos The position to check.
             * @return A vector of players at the given position.
             */
            std::vector<std::shared_ptr<Player>> getPlayersAtPos(raylib::Vector2 pos) const;
        protected:
            std::vector<std::shared_ptr<Team>> _teams;
            std::shared_ptr<ICache> _cache;
        private:
    };
}

#endif /* !TEAMMANAGER_HPP_ */