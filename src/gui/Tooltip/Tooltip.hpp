/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Tooltip
*/

#ifndef TOOLTIP_HPP_
    #define TOOLTIP_HPP_

#include <string>
#include <vector>

#include "Raylib/raylib-cpp.hpp"
#include "Cache/Cache.hpp"

namespace Zappy {
    class Tooltip {
        public:
            Tooltip(std::shared_ptr<ICache> cache);
            ~Tooltip() = default;

            /**
             * @brief Draws the tooltip with the given lines of text.
             * @param lines The lines of text to display in the tooltip.
             */
            void draw(std::vector<std::string> lines);
        protected:
            raylib::Rectangle _rect;
            std::shared_ptr<ICache> _cache;
    };
}

#endif /* !TOOLTIP_HPP_ */