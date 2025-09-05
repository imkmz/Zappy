#include "ADrawable.hpp"

Zappy::ADrawable::ADrawable(std::shared_ptr<ICache> cache, raylib::Vector3 pos)
{
    _cache = cache;
    _pos = pos;
}

std::shared_ptr<Zappy::ICache> Zappy::ADrawable::getCache() const
{
    return _cache;
}

raylib::Vector3 Zappy::ADrawable::getPos() const
{
    return _pos;
}