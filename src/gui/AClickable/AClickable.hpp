/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** AClickable
*/

#ifndef ACLICKABLE_HPP_
    #define ACLICKABLE_HPP_

#include "IClickable/IClickable.hpp"
#include "ICache/ICache.hpp"

namespace Zappy {
    class AClickable : public IClickable {
        public:
            AClickable(const ModelId &id, const std::shared_ptr<ICache> &cache, const raylib::Vector3 &pos);
            ~AClickable() = default;

            virtual void setPos(const raylib::Vector3 &pos) override;
            virtual bool isClicked(const raylib::Ray &ray) const override;
            virtual raylib::RayCollision getCollision(const raylib::Ray &ray) const override;
        protected:
            ModelId _modelId;
            std::shared_ptr<ICache> _cacheB;
            raylib::Vector3 _posB;
    };
}

#endif /* !ACLICKABLE_HPP_ */
