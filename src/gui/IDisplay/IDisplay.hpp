/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** IDisplay
*/

#ifndef IDISPLAY_HPP_
    #define IDISPLAY_HPP_

namespace Zappy {
    class IDisplay {
        public:
            virtual ~IDisplay() = default;
            virtual void displayLoop() = 0;
            virtual void draw() const = 0;
            virtual void update() = 0;
    };
}

#endif /* !IDISPLAY_HPP_ */
