#include "Map.hpp"

Zappy::Map::Map(size_t x, size_t y, std::shared_ptr<ICache> cache) :
    ADrawable(cache, {})
{
    initTiles(x, y);
}

void Zappy::Map::draw(std::shared_ptr<ICamera> camera) const
{
    for (auto &row : _tiles)
        for (auto &tile : row)
            tile->draw(camera);
}

void Zappy::Map::update()
{
    return;
}

std::vector<std::vector<std::shared_ptr<Zappy::Tile>>> Zappy::Map::getTiles() const
{
    return _tiles;
}

std::shared_ptr<Zappy::Tile> Zappy::Map::getTile(size_t x, size_t y) const
{
    if (x < 0 || x > _size.x || y < 0 || y > _size.y)
        return nullptr;
    return _tiles[y][x];
}

void Zappy::Map::initTiles(size_t x, size_t y)
{
    raylib::Vector3 curPos = {0, 0, 0};

    _tiles.clear();
    for (size_t i = 0; i < y; i++) {
        std::vector<std::shared_ptr<Tile>> cur;

        for (size_t j = 0; j < x; j++) {
            cur.push_back(std::make_shared<Tile>(curPos, _cache));
            curPos += {1, 0, 0};
        }
        _tiles.push_back(cur);
        curPos.x = 0;
        curPos += {0, 0, 1};
    }
    _size = raylib::Vector2(x, y);
}

void Zappy::Map::setSize(size_t x, size_t y)
{
    initTiles(x, y);
}

raylib::Vector2 Zappy::Map::getSize() const
{
    return _size;
}
