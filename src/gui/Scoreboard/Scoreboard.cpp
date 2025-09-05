#include "Scoreboard.hpp"

Zappy::Scoreboard::Scoreboard(std::shared_ptr<TeamManager> teams, std::shared_ptr<ICache> cache) :
    ADrawable(cache, {})
{
    _teamManager = teams;
}

void Zappy::Scoreboard::draw(std::shared_ptr<Zappy::ICamera> camera) const
{
    auto scores = getScores();

    _rect.DrawRounded(0.3f, 100, *_cache->getColor(COL_MAIN));
    _rect.DrawRoundedLines(0.3f, 100, 2.0f, *_cache->getColor(COL_ACCENT));

    raylib::DrawText("Scoreboard", _pos.x + 15, _pos.y + 15, 25, *_cache->getColor(COL_ACCENT));
    raylib::DrawText("Team", _pos.x + 15, _pos.y + 55, 25, *_cache->getColor(COL_ACCENT));
    raylib::DrawText("Level", _pos.x + _width - 80, _pos.y + 55, 25, *_cache->getColor(COL_ACCENT));

    float y = 85;
    for (auto &score : scores) {
        std::string lvl = std::to_string(score.second);

        raylib::DrawText(score.first.c_str(), _pos.x + 15, _pos.y + y + 5, 25, *_cache->getColor(COL_ACCENT));
        raylib::DrawText(lvl.c_str(), _pos.x + _width - MeasureText(lvl.c_str(), 25) - 15, _pos.y + y + 5, 25, *_cache->getColor(COL_ACCENT));
        y += 25;
    }
}

void Zappy::Scoreboard::update()
{
    _width = 0;
    auto scores = getScores();
    float height = 30 + scores.size() * 22;

    for (auto &str : scores) {
        float cur = MeasureText(str.first.c_str(), 25);

        if (cur > _width)
            _width = cur;
    }
    _width += 30;
    _width *= 2;
    height += 85;

    _pos = raylib::Vector3(GetScreenWidth() / 2 - _width / 2, GetScreenHeight() / 2 - height / 2, 0);
    _rect = raylib::Rectangle({_pos.x, _pos.y}, {_width, height});
}

std::vector<std::pair<std::string, int>> Zappy::Scoreboard::getScores() const
{
    std::vector<std::pair<std::string, int>> displayVec;

    for (auto &team : _teamManager->getTeams()) {
        std::string cur = team->getName();

        int lvl = 0;

        for (auto &ply : team->getPlayers())
            if (ply->getLevel() > lvl) {
                lvl = ply->getLevel();
            }
        displayVec.push_back({cur, lvl});
    }
    return displayVec;
}
