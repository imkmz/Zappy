/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Map
*/

#ifndef MAP_HPP_
    #define MAP_HPP_

#include <vector>
#include <memory>

#include "Tile/Tile.hpp"

namespace Zappy {
    class Map : public ADrawable {
        public:
            Map(size_t x, size_t y, std::shared_ptr<ICache> cache);
            ~Map() = default;

            /// @brief Loop in 3D Draw context to render map and its components
            void draw(std::shared_ptr<ICamera> camera) const override;
            void update() override;
            /// @brief Gets the tile vector that represents the map
            /// @return Vector of shared pointers for each tile object
            std::vector<std::vector<std::shared_ptr<Tile>>> getTiles() const;
            /// @brief Gets a specific tile, coordinates starts at {1, 1}
            /// @param x The tile's x coordinate
            /// @param y The tile's y coordinate
            /// @return The tile if it exists else returns a nullptr
            std::shared_ptr<Tile> getTile(size_t x, size_t y) const;
            /// @brief Update tiles array for a size
            /// @param x Width
            /// @param y Height
            void initTiles(size_t x, size_t y);
            /// @brief Sets the size of the map
            /// @param x Width
            /// @param y Height
            void setSize(size_t x, size_t y);
            raylib::Vector2 getSize() const;
        protected:
            raylib::Vector2 _size;
            std::vector<std::vector<std::shared_ptr<Tile>>> _tiles;
    };
}

#endif /* !MAP_HPP_ */
