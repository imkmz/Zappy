#include "NetworkUtils.hpp"

Zappy::NetworkError::NetworkError(const std::string &msg)
{
    _message = msg;
}

const char *Zappy::NetworkError::what() const noexcept
{
    return _message.c_str();
}

Zappy::NetworkUtils::NetworkUtils(const std::string &ip, const int port)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_fd == -1)
        throw NetworkError("Socket failed.");

    if (ip == "localhost")
        _ip = inet_addr("127.0.0.1");
    else
        _ip = inet_addr(ip.c_str());
    _port = htons(port);

    _symbolValidity = genSymbolValidity();
    _commands = genCommands();
    _timeUnit = 100;
    _plyMutex = std::make_shared<std::mutex>();

    _ressourceNbMap = {
        {1, FOOD},
        {2, LINEMATE},
        {3, DERAUMERE},
        {4, SIBUR},
        {5, MENDIANE},
        {6, PHIRAS},
        {7, THYSTAME}
    };
}

void Zappy::NetworkUtils::tryConnect()
{
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = _ip;
    addr.sin_port = _port;
    if (connect(_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        throw NetworkError("Connection to server failed.");
}

std::string Zappy::NetworkUtils::receiveMessage()
{
    std::string total;
    std::vector<std::string> cmds;

    while (total.empty() || total.back() != '\n') {
        std::vector<char> cur(256);

        if (recv(_fd, cur.data(), cur.size(), 0) <= 0)
            throw NetworkError("Connection lost.");
        total.append(std::string(cur.begin(), cur.end()));
        total.erase(std::find(total.begin(), total.end(), '\0'), total.end());
    }

    size_t pos = total.find('\n');
    while (pos != std::string::npos && pos != 0) {
        cmds.push_back(total.substr(0, pos + 1));
        total.erase(0, pos + 1);
        pos = total.find('\n');
    }

    for (auto &cmd : cmds)
        _cmdQueue.push(Utils::split(Utils::cleanStr(cmd), ' '));
    return total;
}

int Zappy::NetworkUtils::getFd() const
{
    return _fd;
}

std::unordered_map<Zappy::Symbol, std::function<bool(const std::string&)>> Zappy::NetworkUtils::genSymbolValidity() const
{
    auto rangeIntValidator = [](int min, int max) {
        return [=](const std::string &arg) {
            int val = std::atoi(arg.c_str());
            if (val < min || val > max) {
                return false;
            }
            return val >= min && val <= max;
        };
    };
    auto alwaysTrue = [](const std::string &arg) {
        return true;
    };

    return {
        {X, rangeIntValidator(0, 999999)},
        {Y, rangeIntValidator(0, 999999)},
        {q0, rangeIntValidator(0, 999999)},
        {q1, rangeIntValidator(0, 999999)},
        {q2, rangeIntValidator(0, 999999)},
        {q3, rangeIntValidator(0, 999999)},
        {q4, rangeIntValidator(0, 999999)},
        {q5, rangeIntValidator(0, 999999)},
        {q6, rangeIntValidator(0, 999999)},
        {n, rangeIntValidator(0, 999999)},
        {O, rangeIntValidator(1, 4)},
        {L, rangeIntValidator(1, 8)},
        {e, rangeIntValidator(0, 999999)},
        {T, rangeIntValidator(0, 999999)},
        {N, alwaysTrue},
        {mN, rangeIntValidator(0, 999999)},
        {R, alwaysTrue},
        {M, alwaysTrue},
        {i, rangeIntValidator(0, 999999)}
    };
}

std::unordered_map<std::string, Zappy::SymbolsArguments> Zappy::NetworkUtils::genCommands() const
{
    return {
        {"msz", {X, Y}},
        {"bct", {X, Y, q0, q1, q2, q3, q4, q5, q6}},
        {"tna", {N}},
        {"pnw", {n, X, Y, O, L, N}},
        {"ppo", {n, X, Y, O}},
        {"plv", {n, L}},
        {"pin", {n, X, Y, q0, q1, q2, q3, q4, q5, q6}},
        {"pex", {n}},
        {"pbc", {n, M}},
        {"pic", {X, Y, L, mN}},
        {"pie", {X, Y, R}},
        {"pfk", {n}},
        {"pdr", {n, i}},
        {"pgt", {n, i}},
        {"pdi", {n}},
        {"enw", {e, n, X, Y}},
        {"ebo", {e}},
        {"edi", {e}},
        {"sgt", {T}},
        {"sst", {T}},
        {"seg", {N}},
        {"smg", {M}},
        {"suc", {}},
        {"sbp", {}},
        {"WELCOME", {}}
    };
}

bool Zappy::NetworkUtils::validateCommand(const std::vector<std::string> &cmd) const
{
    auto it = _commands.find(cmd[0]);

    if (it == _commands.end())
        return false;
    for (size_t i = 0; i < it->second.size(); ++i) {
        auto sIt = _symbolValidity.find(it->second[i]);
        if (sIt != _symbolValidity.end()) {
            if (sIt->second(cmd[i]))
                continue;
            return false;
        }
        return false;
    }
    return true;
}

bool Zappy::NetworkUtils::execCommand()
{
    if (_cmdQueue.empty())
        return false;

    std::vector<std::string> cmd = _cmdQueue.front();
    _cmdQueue.pop();

    // if (!validateCommand(cmd)) {
    //     std::cerr << "Invalid command: " << cmd[0] << " with arguments: ";
    //     for (const auto &arg : cmd) {
    //         std::cerr << arg << " ";
    //     }
    //     std::cerr << std::endl;
    //     return false;
    // }

    std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> commandHandlers = {
        {"msz", [this](const auto& args){ msz(args); }},
        {"bct", [this](const auto& args){ bct(args); }},
        {"tna", [this](const auto& args){ tna(args); }},
        {"pnw", [this](const auto& args){ pnw(args); }},
        {"ppo", [this](const auto& args){ ppo(args); }},
        {"plv", [this](const auto& args){ plv(args); }},
        {"pin", [this](const auto& args){ pin(args); }},
        {"pex", [this](const auto& args){ pex(args); }},
        {"pbc", [this](const auto& args){ pbc(args); }},
        {"pic", [this](const auto& args){ pic(args); }},
        {"pie", [this](const auto& args){ pie(args); }},
        {"pfk", [this](const auto& args){ pfk(args); }},
        {"pdr", [this](const auto& args){ pdr(args); }},
        {"pgt", [this](const auto& args){ pgt(args); }},
        {"pdi", [this](const auto& args){ pdi(args); }},
        {"enw", [this](const auto& args){ enw(args); }},
        {"ebo", [this](const auto& args){ ebo(args); }},
        {"edi", [this](const auto& args){ edi(args); }},
        {"sgt", [this](const auto& args){ sgt(args); }},
        {"sst", [this](const auto& args){ sst(args); }},
        {"seg", [this](const auto& args){ seg(args); }},
        {"smg", [this](const auto& args){ smg(args); }},
        {"suc", [this](const auto& args){ suc(args); }},
        {"sbp", [this](const auto& args){ sbp(args); }},
        {"WELCOME", [this](const auto& args){ welcome(args); }},
    };

    auto it = commandHandlers.find(cmd[0]);
    if (it != commandHandlers.end()) {
        if (it->first != "bct")
            _bctCount = 0;
        it->second(std::vector<std::string>(cmd.begin() + 1, cmd.end()));
        return true;
    }
    return false;
}

void Zappy::NetworkUtils::sendToServer(const std::string &str)
{
    std::string response = str;

    response.push_back('\n');
    send(_fd, response.c_str(), response.size(), 0);
}

void Zappy::NetworkUtils::changeTimeUnit(int unit)
{
    if (_waitForTimeChange)
        return;
    unit = std::clamp(unit, 1, 9999);

    std::stringstream ss;
    ss << "sst " << unit;
    _waitForTimeChange = true;
    sendToServer(ss.str());
}

int Zappy::NetworkUtils::getTimeUnit() const
{
    return _timeUnit;
}

std::optional<std::tuple<raylib::Vector2, int, int, int, std::string>> Zappy::NetworkUtils::popPlyRequest()
{
    std::lock_guard<std::mutex> lock(*_plyMutex);

    if (_plyRequest.empty())
        return std::nullopt;
    auto request = _plyRequest.front();
    _plyRequest.pop();
    return request;
}

std::string Zappy::NetworkUtils::getBroadcast() const
{
    return _broadcast;
}

void Zappy::NetworkUtils::setTeamManager(std::shared_ptr<TeamManager> tm)
{
    _teamManager = tm;
}

void Zappy::NetworkUtils::setMap(std::shared_ptr<Map> map)
{
    _map = map;
}

void Zappy::NetworkUtils::msz(const std::vector<std::string>& args)
{
    if (args.size() != 2)
        return;

    int x = std::atoi(args[0].c_str());
    int y = std::atoi(args[1].c_str());

    if (x <= 0 || y <= 0 || (x == _map->getSize().x && y == _map->getSize().y))
        return;
    _map->setSize(x, y);
}

void Zappy::NetworkUtils::bct(const std::vector<std::string>& args)
{
    if (args.size() != 9)
        return;

    int x = std::atoi(args[0].c_str());
    int y = std::atoi(args[1].c_str());
    auto tile = _map->getTile(x, y);

    if (!tile)
        return;

    tile->updateResources({
        {Resources::FOOD, std::atoi(args[2].c_str())},
        {Resources::LINEMATE, std::atoi(args[3].c_str())},
        {Resources::DERAUMERE, std::atoi(args[4].c_str())},
        {Resources::SIBUR, std::atoi(args[5].c_str())},
        {Resources::MENDIANE, std::atoi(args[6].c_str())},
        {Resources::PHIRAS, std::atoi(args[7].c_str())},
        {Resources::THYSTAME, std::atoi(args[8].c_str())},
    });
}

void Zappy::NetworkUtils::tna(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;
    if (!_teamManager->getTeam(args[0]))
        _teamManager->registerTeam(args[0]);
}

void Zappy::NetworkUtils::pnw(const std::vector<std::string>& args)
{
    if (args.size() != 6)
        return;

    int n = std::atoi(args[0].c_str());
    int x = std::atoi(args[1].c_str());
    int y = std::atoi(args[2].c_str());
    int o = std::atoi(args[3].c_str());
    int l = std::atoi(args[4].c_str());

    if (x < 0 || y < 0 || o < 1 || o > 4 || l < 1 || l > 8)
        return;

    {
        std::lock_guard<std::mutex> lock(*_plyMutex);
        _plyRequest.push({{static_cast<float>(x), static_cast<float>(y)}, n, o, l, args[5]});
    }
}

void Zappy::NetworkUtils::ppo(const std::vector<std::string>& args)
{
    if (args.size() != 4)
        return;

    int n = std::atoi(args[0].c_str());
    int x = std::atoi(args[1].c_str());
    int y = std::atoi(args[2].c_str());
    int o = std::atoi(args[3].c_str());

    auto ply = _teamManager->getPlayer(n);
    if (!ply || x < 0 || y < 0 || o < 1 || o > 4)
        return;

    if (ply->getPos() != raylib::Vector3(x, 0.5, y))
        ply->setState(WALKING);
    ply->setPosition(x, y);
    ply->setOrientation(o);
}

void Zappy::NetworkUtils::plv(const std::vector<std::string>& args)
{
    if (args.size() != 2)
        return;

    int n = std::atoi(args[0].c_str());
    int l = std::atoi(args[1].c_str());

    auto ply = _teamManager->getPlayer(n);
    if (!ply || l < 1 || l > 8)
        return;
    ply->setLevel(l);
}

void Zappy::NetworkUtils::pin(const std::vector<std::string>& args)
{
    if (args.size() != 10)
        return;

    int n = std::atoi(args[0].c_str());
    auto ply = _teamManager->getPlayer(n);
    if (!ply)
        return;

    std::unordered_map<Resources, int> inventory = {
        {FOOD, std::atoi(args[3].c_str())},
        {LINEMATE, std::atoi(args[4].c_str())},
        {DERAUMERE, std::atoi(args[5].c_str())},
        {SIBUR, std::atoi(args[6].c_str())},
        {MENDIANE, std::atoi(args[7].c_str())},
        {PHIRAS, std::atoi(args[8].c_str())},
        {THYSTAME, std::atoi(args[9].c_str())},
    };

    ply->setInventory(inventory);
}

void Zappy::NetworkUtils::pex(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;

    auto ply = _teamManager->getPlayer(std::atoi(args[0].c_str()));
    if (!ply)
        return;
    ply->setState(EJECTING);
}

void Zappy::NetworkUtils::pbc(const std::vector<std::string>& args)
{
    if (args.size() != 2)
        return;

    int n = std::atoi(args[0].c_str());

    auto ply = _teamManager->getPlayer(n);
    if (!ply)
        return;
    ply->speak(args[1]);
}

void Zappy::NetworkUtils::pic(const std::vector<std::string>& args)
{
    if (args.size() < 3)
        return;

    int x = std::atoi(args[0].c_str());
    int y = std::atoi(args[1].c_str());
    auto tile = _map->getTile(x, y);

    if (!tile)
        return;

    for (size_t i = 3; i < args.size(); i++) {
        auto ply = _teamManager->getPlayer(std::atoi(args[i].c_str()));
        if (!ply)
            continue;
        ply->startIncantation(x, y);
        ply->setState(INCANTING);
    }
}

void Zappy::NetworkUtils::pie(const std::vector<std::string>& args)
{
    if (args.size() != 3)
        return;

    int x = std::atoi(args[0].c_str());
    int y = std::atoi(args[1].c_str());
    int result = std::atoi(args[2].c_str());

    auto tile = _map->getTile(x, y);
    if (!tile)
        return;

    auto plys = _teamManager->getPlayersAtPos({static_cast<float>(x), static_cast<float>(y)});
    for (auto &ply : plys)
        ply->endIncantation(result);
}

void Zappy::NetworkUtils::pfk(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;
    _teamManager->getPlayer(atoi(args[0].c_str()))->setState(EGG_LAYING);
}

void Zappy::NetworkUtils::pdr(const std::vector<std::string>& args) {
    if (args.size() != 2)
        return;

    auto ply = _teamManager->getPlayer(atoi(args[0].c_str()));
    auto res = _ressourceNbMap.find(atoi(args[1].c_str()));
    auto tile = _map->getTile(ply->getPos().x, ply->getPos().z);

    if (res == _ressourceNbMap.end() || !ply)
        return;
    ply->setState(DROPPING);
    tile->getInventory()->addItem(res->second);
    ply->getInventory()->removeItem(res->second);
}

void Zappy::NetworkUtils::pgt(const std::vector<std::string>& args) {
    if (args.size() != 2)
        return;

    auto ply = _teamManager->getPlayer(atoi(args[0].c_str()));
    if (!ply)
        return;
    auto res = _ressourceNbMap.find(atoi(args[1].c_str()));
    auto tile = _map->getTile(ply->getPos().x, ply->getPos().z);

    if (res == _ressourceNbMap.end() || !ply)
        return;
    ply->setState(COLLECTING);
    tile->getInventory()->removeItem(res->second);
    ply->getInventory()->addItem(res->second);
}

void Zappy::NetworkUtils::pdi(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;

    auto ply = _teamManager->getPlayer(atoi(args[0].c_str()));
    if (!ply)
        return;
    ply->setDying();
}

void Zappy::NetworkUtils::enw(const std::vector<std::string>& args)
{
    if (args.size() != 4)
        return;

    int e = std::atoi(args[0].c_str());
    int n = std::atoi(args[1].c_str());
    int x = std::atoi(args[2].c_str());
    int y = std::atoi(args[3].c_str());

    auto ply = _teamManager->getPlayer(n);
    std::shared_ptr<Team> team;

    if (ply)
        team = _teamManager->getTeam(ply->getTeam());
    else
        team = _teamManager->getDefaultTeam();
    team->addEgg(e, x, y);
}

void Zappy::NetworkUtils::ebo(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;

    int e = std::atoi(args[0].c_str());
    auto pos = _teamManager->getEgg(e)->getPos();
    int orientation = (rand() % 4) + 1;
    int n = _teamManager->getPlayers().size() + 1;

    std::string teamName = _teamManager->getEgg(e)->getTeam();
    {
        std::lock_guard<std::mutex> lock(*_plyMutex);
        _plyRequest.push({{pos.x, pos.y}, n, orientation, 1, teamName});
    }
    _teamManager->removeEgg(_teamManager->getEgg(e)->getId());
}

void Zappy::NetworkUtils::edi(const std::vector<std::string>& args)
{
    if (args.size() != 1)
        return;
    _teamManager->removeEgg(atoi(args[0].c_str()));
}

void Zappy::NetworkUtils::sgt(const std::vector<std::string>& args) {
    if (args.size() != 1)
        return;
    _waitForTimeChange = false;
    _timeUnit = std::atoi(args[0].c_str());
}

void Zappy::NetworkUtils::sst(const std::vector<std::string>& args) {
    if (args.size() != 1)
        return;
    _waitForTimeChange = false;
    _timeUnit = std::atoi(args[0].c_str());
}

void Zappy::NetworkUtils::seg(const std::vector<std::string>& args) {
    if (args.size() != 1)
        return;

    _broadcast = "Team: " + args[0] + " won!";
}

void Zappy::NetworkUtils::smg(const std::vector<std::string>& args) {
    if (args.size() != 1)
        return;
    _broadcast = args[0];
}

void Zappy::NetworkUtils::suc(const std::vector<std::string>& args) {
    std::cout << "suc was executed with args: ";
    for (const auto& arg : args) std::cout << arg << " ";
    std::cout << std::endl;
}

void Zappy::NetworkUtils::sbp(const std::vector<std::string>& args) {
    std::cout << "sbp was executed" << std::endl;
}

void Zappy::NetworkUtils::welcome(const std::vector<std::string> &args)
{
    sendToServer("GRAPHIC");
}
