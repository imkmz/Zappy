/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Team
*/

#ifndef TEAM_HPP_
    #define TEAM_HPP_

#include <vector>
#include <memory>

#include "Player/Player.hpp"
#include "Egg/Egg.hpp"
#include "Camera/Camera.hpp"
#include "ADrawable/ADrawable.hpp"

namespace Zappy {
    class Team : public ADrawable {
        public:
            Team(const std::string &name, std::shared_ptr<ICache> cache);
            ~Team() = default;

            /**
             * @brief Draws all players and eggs of the team using the given camera.
             * @param camera The camera to use for rendering.
             */
            void draw(std::shared_ptr<Zappy::ICamera> camera) const override;
            void update() override;

            std::string getName() const;

            std::vector<std::shared_ptr<Player>> getPlayers() const;
            std::vector<std::shared_ptr<Egg>> getEggs() const;

            /**
             * @brief Adds a player to the team with a specific id.
             * @param id The player's id.
             * @param x The player's x position.
             * @param y The player's y position.
             * @param orientation The player's orientation.
             * @param level The player's level.
             */
            void addPlayer(int id, int x, int y, int orientation, int level);

            /**
             * @brief Adds a player to the team with an automatically assigned id.
             * @param x The player's x position.
             * @param y The player's y position.
             * @param orientation The player's orientation.
             * @param level The player's level.
             */
            void addPlayer(int x, int y, int orientation, int level);

            /**
             * @brief Removes a player from the team by id.
             * @param id The id of the player to remove.
             */
            void removePlayer(int id);

            std::shared_ptr<Player> getPlayer(int id) const;

            /**
             * @brief Adds an egg to the team.
             * @param id The egg's id.
             * @param x The egg's x position.
             * @param y The egg's y position.
             */
            void addEgg(int id, int x, int y);

            /**
             * @brief Removes an egg from the team by id.
             * @param id The id of the egg to remove.
             */
            void removeEgg(int id);

            std::shared_ptr<Egg> getEgg(int id) const;
        private:
            std::string _name;
            std::shared_ptr<ICache> _cache;

            std::vector<std::shared_ptr<Player>> _players;
            std::vector<std::shared_ptr<Egg>> _eggs;
    };
}

#endif /* !TEAM_HPP_ */