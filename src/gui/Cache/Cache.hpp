/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Cache
*/

#ifndef CACHE_HPP_
    #define CACHE_HPP_

#include <unordered_map>

#include "ICache/ICache.hpp"

namespace Zappy {
    class Cache : public ICache {
        public:
            /// @brief Simple class to cache models to optimize their rendering
            Cache();
            ~Cache() = default;

            raylib::Vector3 getOffset(const ModelId &id) const override;
            raylib::Vector3 getOffset(const Resources &id) const override;
            double getRotation(const ModelId &id) const override;
            double getRotation(const Resources &id) const override;
            std::shared_ptr<raylib::Texture2D> getTexture(const ModelId &id) const override;
            std::shared_ptr<raylib::Texture2D> getTexture(const Resources &id) const override;
            /// @brief Gets the specific cached color using it's identifier
            /// @param id The color's id
            /// @return The color
            std::shared_ptr<raylib::Color> getColor(const ColorId &id) const override;
            /// @brief Gets the specific model using it's identifier
            /// @param id The model id represented by a ModelId enum
            /// @return The model
            std::shared_ptr<raylib::Model> getModel(const ModelId &id) const override;
            /// @brief Gets the specific resource's model using it's identifier (exists for legacy purposes)
            /// @param res Resource's enum
            /// @return Resource's model
            std::shared_ptr<raylib::Model> getModel(const Resources &res) const override;
            /// @brief Gets the specific scale for a model using it's identifier
            /// @param id The model id represented by a ModelId enum
            /// @return The model's scale
            double getScale(const ModelId &id) const override;
            /// @brief Gets the specific resource's scale using it's identifier (exists for legacy purposes)
            /// @param res Resource's enum
            /// @return Resource's scale
            double getScale(const Resources &res) const override;
        protected:
            std::unordered_map<ModelId, std::shared_ptr<raylib::Model>> _modelMap;
            std::unordered_map<ModelId, double> _scaleMap;
            std::unordered_map<ModelId, double> _rotMap;
            std::unordered_map<Resources, ModelId> _correspondanceMap;
            std::unordered_map<ColorId, std::shared_ptr<raylib::Color>> _colorMap;
            std::unordered_map<ModelId, std::shared_ptr<raylib::Texture2D>> _textureMap;
            std::unordered_map<ModelId, raylib::Vector3> _offsetMap;
        private:
    };
}

#endif /* !CACHE_HPP_ */
