/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Tile
*/

#ifndef TILE_HPP_
    #define TILE_HPP_

#include "Inventory/Inventory.hpp"
#include "Cache/Cache.hpp"
#include "AClickable/AClickable.hpp"
#include "ADrawable/ADrawable.hpp"

namespace Zappy {
    class Tile : public AClickable, public ADrawable {
        public:
            Tile(raylib::Vector3 pos, const std::shared_ptr<ICache> &cache);
            ~Tile() = default;

            /// @brief Loop in the 3D Draw context to render the tile
            void draw(std::shared_ptr<ICamera> camera) const override;
            void update() override;
            /// @brief Returns the outline color of the tile (depending on it's hover/selection status)
            /// @return Tile's outline color
            Color getOutline() const;
            std::shared_ptr<Inventory> getInventory() const;
            /// @brief Updates the tile resource number
            /// @param resources Map for the resources enum and number
            void updateResources(const std::unordered_map<Resources, int> &resources);
            virtual std::vector<std::string> getToolTip() const override;
        protected:
            std::shared_ptr<Inventory> _inventory;
            raylib::Vector3 _size = {1, 1, 1};
            bool _isSelected = false;
            bool _isHovered = false;
    };
}

#endif /* !TILE_HPP_ */
