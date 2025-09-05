/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Display
*/

#ifndef DISPLAY_HPP_
    #define DISPLAY_HPP_

#include "Raylib/raylib-cpp.hpp"
#include "NetworkUtils/NetworkUtils.hpp"
#include "Tooltip/Tooltip.hpp"
#include "Camera/Camera.hpp"
#include "Map/Map.hpp"
#include "Utils/Utils.hpp"
#include "Scoreboard/Scoreboard.hpp"
#include "TeamManager/TeamManager.hpp"
#include "IClickable/IClickable.hpp"
#include "IDisplay/IDisplay.hpp"

#include <thread>
#include <queue>

namespace Zappy {

    enum stat {
        NONE,
        PLAYER,
        EGG
    };

    struct clickInfoStruct {
        enum stat isClicked;
        int playerNumber;
        int eggNumber;
    };

    class Display : public IDisplay {
        public:
            Display(size_t x, size_t y, std::shared_ptr<INetwork> network);
            ~Display() = default;

            /// @brief Loop ran while the window is open
            void displayLoop() override;
            /// @brief Loop in the drawing context
            void draw() const override;
            /// @brief Loop in the 3D context
            void render3D() const;
            /// @brief Loop in the 2D context
            void render2D() const;
            /// @brief Loop for the logic
            void update() override;
            /// @brief Check if a clickable object is clicked
            void updateClick();
        protected:
            bool _serverDown = false;
            raylib::Window _window;
            std::shared_ptr<ICamera> _camera;
            std::shared_ptr<INetwork> _network;
            std::shared_ptr<ICache> _cache;
            std::shared_ptr<Map> _map;
            std::shared_ptr<TeamManager> _teamManager;
            std::shared_ptr<Scoreboard> _scoreboard;
            std::shared_ptr<Tooltip> _tooltip;
            std::shared_ptr<IClickable> _click;

        private:
    };
}

#endif /* !DISPLAY_HPP_ */
