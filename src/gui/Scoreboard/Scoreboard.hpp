/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Scoreboard
*/

#ifndef SCOREBOARD_HPP_
#define SCOREBOARD_HPP_

#include "TeamManager/TeamManager.hpp"
#include "ADrawable/ADrawable.hpp"

namespace Zappy
{
    class Scoreboard : public ADrawable
    {
        public:
            Scoreboard(std::shared_ptr<TeamManager> teams, std::shared_ptr<ICache> cache);
            ~Scoreboard() = default;

            /// @brief Draws the scoreboard in a 2D context
            void draw(std::shared_ptr<Zappy::ICamera> camera) const override;
            void update() override;
            /// @brief Used to generated a string containing all the maximum levels of each team
            /// @return An array of string containing the formatted scores
            std::vector<std::pair<std::string, int>> getScores() const;

        private:
            raylib::Rectangle _rect;
            std::shared_ptr<TeamManager> _teamManager;
            float _width;
    };
}

#endif /* !SCOREBOARD_HPP_ */
