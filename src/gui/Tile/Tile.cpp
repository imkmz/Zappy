#include "Tile.hpp"

Zappy::Tile::Tile(raylib::Vector3 pos, const std::shared_ptr<ICache> &cache) :
    AClickable(M_TILE, cache, {pos.x, 0, pos.z}),
    ADrawable(cache, {pos.x, 0, pos.z})
{
    _inventory = std::make_shared<Inventory>();
}

void Zappy::Tile::draw(std::shared_ptr<Zappy::ICamera> camera) const
{
    auto model = _cache->getModel(M_TILE);
    auto texture = _cache->getTexture(M_TILE);

    if (model && model->IsValid() && texture && texture->IsValid()) {
        model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
        model->Draw(_pos, _cache->getScale(M_TILE));
    }

    auto resPos = raylib::Vector3(-_size.x / 2 + 0.1f, _size.y / 2, -_size.z / 2 + 0.1f);

    for (auto &resource : _inventory->getInventory()) {
        auto model = _cache->getModel(resource.first);
        if (!model || !model->IsValid())
            return;
        auto bbox = model->GetBoundingBox();
        raylib::Vector3 bbMin = bbox.min;
        raylib::Vector3 bbMax = bbox.max;
        raylib::Vector3 size = (bbMax - bbMin) * _cache->getScale(resource.first);

        for (int i = 0; i < resource.second; i++) {
            raylib::Vector3 drawPos = _pos + resPos + raylib::Vector3(0, i * size.y, 0);

            drawPos += _cache->getOffset(resource.first);
            model->transform = raylib::Matrix::RotateX(_cache->getRotation(resource.first));
            auto texture = _cache->getTexture(resource.first);
            if (texture)
                model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
            model->Draw(drawPos, _cache->getScale(resource.first));
        }
        resPos.x += size.x;
    }
}

std::shared_ptr<Zappy::Inventory> Zappy::Tile::getInventory() const
{
    return _inventory;
}

std::vector<std::string> Zappy::Tile::getToolTip() const
{
    std::vector<std::string> tt;

    tt.push_back("Tile: " + std::to_string(static_cast<int>(_pos.x)) + ", " + std::to_string(static_cast<int>(_pos.z)));
    tt.push_back("");
    tt.push_back("Content:");
    tt.push_back("");
    for (auto &item : _inventory->getContent())
        tt.push_back(item);
    return tt;
}

void Zappy::Tile::update()
{
    return;
}

Color Zappy::Tile::getOutline() const
{
    if (_isSelected)
        return BLUE;
    if (_isHovered)
        return YELLOW;
    return BLACK;
}

void Zappy::Tile::updateResources(const std::unordered_map<Resources, int> &resources)
{
    _inventory->setInventory(resources);
}
