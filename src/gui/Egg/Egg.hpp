/*
** EPITECH PROJECT, 2025
** Egg.hpp
** File description:
** Egg.hpp
*/

#ifndef EGG_HPP
#define EGG_HPP

#include <memory>
#include <string>

#include "AClickable/AClickable.hpp"
#include "Player/Player.hpp"
#include "ADrawable/ADrawable.hpp"

namespace Zappy
{
    class Egg : public AClickable, public ADrawable
    {
        public:
            Egg(int id, float x, float y, std::string team, std::shared_ptr<ICache> cache);
            ~Egg() = default;

            /// @brief Handles the drawing for the egg
            void draw(std::shared_ptr<Zappy::ICamera> camera) const override;
            void update() override;

            std::string getTeam() const;
            int getId() const;

            /**
             * @brief Gets the tooltip information for the egg.
             * @return A vector of strings representing the tooltip.
             */
            virtual std::vector<std::string> getToolTip() const override;

        private:
            int _id;
            std::string _team;
    };
}

#endif // EGG_HPP