#include "Cache.hpp"

Zappy::Cache::Cache()
{
    _modelMap = {
        {M_FOOD, std::make_shared<raylib::Model>("assets/models/burger.glb")},
        {M_LINEMATE, std::make_shared<raylib::Model>("assets/models/milk.glb")},
        {M_DERAUMERE, std::make_shared<raylib::Model>("assets/models/ball.glb")},
        {M_SIBUR, std::make_shared<raylib::Model>("assets/models/cardboard.glb")},
        {M_MENDIANE, std::make_shared<raylib::Model>("assets/models/feather.glb")},
        {M_PHIRAS, std::make_shared<raylib::Model>("assets/models/fish.glb")},
        {M_THYSTAME, std::make_shared<raylib::Model>("assets/models/mouse.glb")},
        {M_PLAYER, std::make_shared<raylib::Model>("assets/models/cat.glb")},
        {M_TILE, std::make_shared<raylib::Model>("assets/models/tile.obj")},
        {M_EGG, std::make_shared<raylib::Model>("assets/models/egg.glb")},
    };

    _scaleMap = {
        {M_FOOD, 0.04},
        {M_LINEMATE, 0.05},
        {M_DERAUMERE, 0.04},
        {M_SIBUR, 0.1},
        {M_MENDIANE, 0.09},
        {M_PHIRAS, 0.008},
        {M_THYSTAME, 0.15},
        {M_PLAYER, 0.01},
        {M_TILE, 0.5},
        {M_EGG, 1},
    };

    _offsetMap = {
        {M_FOOD, raylib::Vector3(0.03, 0, 0)},
        {M_LINEMATE, raylib::Vector3(0.01, 0, 0)},
        {M_DERAUMERE, raylib::Vector3(0.005, 0.035, 0)},
        {M_SIBUR, raylib::Vector3(0, 0.02, 0)},
        {M_MENDIANE, raylib::Vector3(-0.2, 0.03, 0)},
        {M_PHIRAS, raylib::Vector3(0.28, 0, 0)},
        {M_THYSTAME, raylib::Vector3(0, 0, 0)},
        {M_PLAYER, raylib::Vector3(0, 0, 0)},
        {M_TILE, raylib::Vector3(0, 0, 0)},
        {M_EGG, raylib::Vector3(0, 0, 0)},
    };

    _textureMap = {
        {M_TILE, std::make_shared<raylib::Texture>("assets/textures/block_bits_texture.png")},
        {M_FOOD, std::make_shared<raylib::Texture>("assets/textures/restaurantbits_texture.png")},
    };

    _colorMap = {
        {COL_MAIN, std::make_shared<raylib::Color>(50, 46, 46)},
        {COL_ACCENT, std::make_shared<raylib::Color>(238, 164, 187)},
        {COL_SKY, std::make_shared<raylib::Color>(232, 249, 255)},
    };

    _correspondanceMap = {
        {Resources::FOOD, M_FOOD},
        {Resources::LINEMATE, M_LINEMATE},
        {Resources::DERAUMERE, M_DERAUMERE},
        {Resources::SIBUR, M_SIBUR},
        {Resources::MENDIANE, M_MENDIANE},
        {Resources::PHIRAS, M_PHIRAS},
        {Resources::THYSTAME, M_THYSTAME},
    };
}

raylib::Vector3 Zappy::Cache::getOffset(const ModelId &id) const
{
    auto it = _offsetMap.find(id);

    if (it != _offsetMap.end())
        return it->second;
    return raylib::Vector3(0, 0, 0);
}

raylib::Vector3 Zappy::Cache::getOffset(const Resources &id) const
{
    auto it = _correspondanceMap.find(id);

    if (it != _correspondanceMap.end())
        return getOffset(it->second);
    return raylib::Vector3(0, 0, 0);
}

double Zappy::Cache::getRotation(const ModelId &id) const
{
    auto it = _rotMap.find(id);

    if (it != _rotMap.end())
        return it->second;
    return 0.0;
}

double Zappy::Cache::getRotation(const Resources &id) const
{
    auto it = _correspondanceMap.find(id);

    if (it != _correspondanceMap.end())
        return getRotation(it->second);
    return 0.0;
}

std::shared_ptr<raylib::Texture> Zappy::Cache::getTexture(const ModelId &id) const
{
    auto it = _textureMap.find(id);

    if (it != _textureMap.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<raylib::Texture2D> Zappy::Cache::getTexture(const Resources &id) const
{
    auto it = _correspondanceMap.find(id);

    if (it != _correspondanceMap.end())
        return getTexture(it->second);
    return nullptr;
}

std::shared_ptr<raylib::Color> Zappy::Cache::getColor(const ColorId &id) const
{
    auto it = _colorMap.find(id);

    if (it != _colorMap.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<raylib::Model> Zappy::Cache::getModel(const ModelId &id) const
{
    auto it = _modelMap.find(id);

    if (it != _modelMap.end())
        return it->second;
    return nullptr;
}

std::shared_ptr<raylib::Model> Zappy::Cache::getModel(const Resources &res) const
{
    auto it = _correspondanceMap.find(res);

    if (it != _correspondanceMap.end())
        return getModel(it->second);
    return nullptr;
}

double Zappy::Cache::getScale(const ModelId &id) const
{
    auto it = _scaleMap.find(id);

    if (it != _scaleMap.end())
        return it->second;
    return 1.0;
}

double Zappy::Cache::getScale(const Resources &res) const
{
    auto it = _correspondanceMap.find(res);

    if (it != _correspondanceMap.end())
        return getScale(it->second);
    return 1.0;
}
