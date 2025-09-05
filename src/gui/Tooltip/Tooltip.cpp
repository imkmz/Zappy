#include "Tooltip.hpp"

Zappy::Tooltip::Tooltip(std::shared_ptr<ICache> cache)
{
    _cache = cache;
}

void Zappy::Tooltip::draw(std::vector<std::string> lines)
{
    float width = 0.0f;
    float height = 15 + lines.size() * 25;

    for (auto &str : lines) {
        float cur = MeasureText(str.c_str(), 25);

        if (cur > width)
            width = cur;
    }
    width += 30;

    raylib::Vector2 pos = {15, 15};
    _rect = raylib::Rectangle(pos, {width, height});
    _rect.DrawRounded(0.1f, 100, *_cache->getColor(COL_MAIN));
    _rect.DrawRoundedLines(0.1f, 100, 2.5f, *_cache->getColor(COL_ACCENT));

    float y = 5;
    for (auto &score : lines) {
        raylib::DrawText(score.c_str(), pos.x + 15, pos.y + y + 5, 25, *_cache->getColor(COL_ACCENT));
        y += 25;
    }
}