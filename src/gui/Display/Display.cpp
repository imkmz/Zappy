#include "Display.hpp"

Zappy::Display::Display(size_t x, size_t y, std::shared_ptr<INetwork> network) :
    _window(GetScreenWidth(), GetScreenHeight(), "Zappy!"),
    _camera(std::make_shared<Camera>()),
    _cache(std::make_shared<Cache>()),
    _map(std::make_shared<Map>(0, 0, _cache))
{
    _window.SetTargetFPS(60);
    _window.ToggleBorderless();
    _window.SetFullscreen(true);
    _network = network;
    _teamManager = std::make_shared<TeamManager>(_cache);
    _network->setTeamManager(_teamManager);
    _network->setMap(_map);
    _scoreboard = std::make_shared<Scoreboard>(_teamManager, _cache);
    _tooltip = std::make_shared<Tooltip>(_cache);

    std::thread networkThread([this](){
        while (!_window.ShouldClose())
            try {
                _network->receiveMessage();
            } catch (const Zappy::NetworkError &e) {
                _serverDown = true;
                return;
            }
    });
    networkThread.detach();

    std::thread commandThread([this](){
        while (!_window.ShouldClose())
            _network->execCommand();
    });
    commandThread.detach();
}

void Zappy::Display::displayLoop()
{
    while (!_window.ShouldClose())
    {
        update();
        draw();
    }
}

void Zappy::Display::draw() const
{
    BeginDrawing();

        _camera->beginMode();
            render3D();
        _camera->endMode();
        render2D();

    EndDrawing();
}

void Zappy::Display::render3D() const
{
    _map->draw(_camera);
    _teamManager->draw(_camera);
}

void Zappy::Display::render2D() const
{
    _window.DrawFPS(GetScreenWidth() - 90, 10);

    std::string f = "Game Frequency: " + std::to_string(_network->getTimeUnit());

    raylib::DrawText(f.c_str(), 10, GetScreenHeight() - 20, 15, *_cache->getColor(COL_MAIN));
    if (_network->getBroadcast() != "") {
        raylib::Vector2 textPos = {GetScreenWidth() / 2.0f, 25};
        int textWidth = MeasureText(_network->getBroadcast().c_str(), 25);

        raylib::DrawText(_network->getBroadcast().c_str(), textPos.x - textWidth / 2, textPos.y, 25, *_cache->getColor(COL_MAIN));
    }
    if (IsKeyDown(KEY_TAB)) {
        _scoreboard->update();
        _scoreboard->draw(_camera);
    }
    if (_click)
        _tooltip->draw(_click->getToolTip());
}

void Zappy::Display::update()
{
    if (_serverDown)
        throw Zappy::NetworkError("Server is down.");
    auto request = _network->popPlyRequest();
    if (request.has_value()) {
        auto [pos, n, o, l, team] = request.value();
        _teamManager->getTeam(team)->addPlayer(n, pos.x, pos.y, o, l);
    }

    static std::chrono::high_resolution_clock::time_point lastUpdate;
    auto now = std::chrono::high_resolution_clock::now();

    _window.ClearBackground(*_cache->getColor(COL_SKY));

    _camera->update();
    for (auto &ply : _teamManager->getPlayers())
        ply->update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        updateClick();

    if (now - lastUpdate < std::chrono::milliseconds(100))
        return;
    lastUpdate = std::chrono::high_resolution_clock::now();
    if (IsKeyDown(KEY_KP_ADD))
        _network->changeTimeUnit(_network->getTimeUnit() + 5);
    if (IsKeyDown(KEY_KP_SUBTRACT))
        _network->changeTimeUnit(_network->getTimeUnit() - 5);
}

void Zappy::Display::updateClick()
{
    Vector2 mousePosition = raylib::Mouse::GetPosition();
    raylib::Ray ray = _camera->getCamera().GetMouseRay(mousePosition);
    std::vector<std::pair<std::shared_ptr<IClickable>, raylib::RayCollision>> hits;
    std::pair<std::shared_ptr<IClickable>,raylib::RayCollision> closest = {nullptr, {false, 99999, {0, 0, 0}, {0, 0, 0}}};

    for (auto &ply : _teamManager->getPlayers())
        if (ply->isClicked(ray))
            hits.push_back({ply, ply->getCollision(ray)});
    for (auto &egg : _teamManager->getEggs())
        if (egg->isClicked(ray))
            hits.push_back({egg, egg->getCollision(ray)});
    for (auto &row : _map->getTiles())
        for (auto &tile : row)
            if (tile->isClicked(ray))
                hits.push_back({tile, tile->getCollision(ray)});

    if (hits.empty()) {
        _click = nullptr;
        return;
    }

    for (auto &hit : hits)
        if (hit.second.distance < closest.second.distance)
            closest = hit;
    _click = closest.first;
}
