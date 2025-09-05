/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** ADrawable
*/

#ifndef ADRAWABLE_HPP_
    #define ADRAWABLE_HPP_

#include "IDrawable/IDrawable.hpp"
#include "Raylib/raylib-cpp.hpp"
#include "ICache/ICache.hpp"

#include <memory>

namespace Zappy {
    class ADrawable : public IDrawable {
        public:
            ADrawable(std::shared_ptr<ICache> cache, raylib::Vector3 pos);
            virtual ~ADrawable() = default;

            raylib::Vector3 getPos() const;
            std::shared_ptr<ICache> getCache() const;

        protected:
            std::shared_ptr<ICache> _cache;
            raylib::Vector3 _pos;
    };
}

#endif /* !ADRAWABLE_HPP_ */
