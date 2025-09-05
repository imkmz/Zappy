#include "AClickable.hpp"

Zappy::AClickable::AClickable(const ModelId &id, const std::shared_ptr<ICache> &cache, const raylib::Vector3 &pos)
{
    _modelId = id;
    _cacheB = cache;
    _posB = pos;
}

void Zappy::AClickable::setPos(const raylib::Vector3 &pos)
{
    _posB = pos;
}

bool Zappy::AClickable::isClicked(const raylib::Ray &ray) const
{
    return getCollision(ray).hit;
}

raylib::RayCollision Zappy::AClickable::getCollision(const raylib::Ray &ray) const
{
    auto mdl = _cacheB->getModel(_modelId);
    float scale = _cacheB->getScale(_modelId);

    raylib::BoundingBox bb = mdl->GetBoundingBox();

    bb.min = Vector3Scale(bb.min, scale);
    bb.max = Vector3Scale(bb.max, scale);

    bb.min = Vector3Add(bb.min, _posB);
    bb.max = Vector3Add(bb.max, _posB);

    raylib::RayCollision collision = ray.GetCollision(bb);
    return collision;
}
