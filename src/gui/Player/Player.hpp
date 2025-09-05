/*
** EPITECH PROJECT, 2025
** player.hpp
** File description:
** player.hpp
*/

#ifndef PLAYER_HPP
    #define PLAYER_HPP

#include "Inventory/Inventory.hpp"
#include "Cache/Cache.hpp"
#include "Camera/Camera.hpp"
#include "AClickable/AClickable.hpp"
#include "ADrawable/ADrawable.hpp"
#include "AnimManager/AnimManager.hpp"

#include <memory>
#include <chrono>
#include <queue>

namespace Zappy
{
    class Player : public AClickable, public ADrawable
    {
    public:
        Player(int id, int x, int y, int orientation, int level, std::string team, std::shared_ptr<ICache> cache);
        ~Player() = default;

        /**
         * @brief Draws the player using the given camera.
         * @param camera The camera to use for rendering.
         */
        void draw(std::shared_ptr<Zappy::ICamera> camera) const override;
        void update() override;

        /**
         * @brief Makes the player display a message above their head.
         * @param msg The message to display.
         */
        void speak(const std::string &msg);

        /**
         * @brief Starts the incantation animation at the given position.
         * @param x The x position of the incantation.
         * @param y The y position of the incantation.
         */
        void startIncantation(int x, int y);

        /**
         * @brief Ends the incantation animation.
         * @param result The result of the incantation (success or failure).
         */
        void endIncantation(bool result);

        /**
         * @brief Gets the tooltip information for the player.
         * @return A vector of strings representing the tooltip.
         */
        virtual std::vector<std::string> getToolTip() const override;

        void setInventory(std::unordered_map<Resources, int> inventory);
        void setPosition(int x, int y);
        void setDying();
        std::chrono::high_resolution_clock::time_point getDeathTime() const;
        bool isDying() const;
        void setLevel(int level);
        void setState(const State &s);
        void setOrientation(int orientation);

        std::shared_ptr<Zappy::Inventory>  getInventory() const;
        int getLevel() const;
        int getHealth() const;
        int getId() const;
        std::string getTeam() const;

    private:
        std::string _message;
        std::chrono::_V2::system_clock::time_point _lastSpeach;

        std::chrono::high_resolution_clock::time_point _deathTime;
        bool _isDying = false;
        int _id;
        int _orientation;
        int _level = 0;
        int _health = 10;
        std::string _team;
        std::shared_ptr<Inventory> _inventory;
        std::shared_ptr<IAnimManager> _animManager;
        bool _isInIncantation;
    };
}

#endif // PLAYER_HPP