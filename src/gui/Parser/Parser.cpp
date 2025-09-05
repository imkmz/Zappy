#include "Parser.hpp"

Zappy::ParsingError::ParsingError(const std::string &msg)
{
    _message = msg;
}

const char *Zappy::ParsingError::what() const noexcept
{
    return _message.c_str();
}

Zappy::Config Zappy::Parser::parseArguments(const int argc, const char **argv)
{
    int port;
    std::string ip;

    if (argc != 5)
        throw ParsingError("Invalid number of arguments.");
    if (static_cast<std::string>(argv[1]) == "-h") {
        printUsage();
        return {};
    }
    for (size_t i = 0; argv[i]; ++i) {
        if (static_cast<std::string>(argv[i]) == "-p" && argv[i + 1])
            port = std::stoi(argv[++i]);
        if (static_cast<std::string>(argv[i]) == "-h" && argv[i + 1])
            ip = argv[++i];
    }
    return {ip, port};
}

void Zappy::Parser::printUsage()
{
    std::cout << "USAGE: " << "./zappy_gui -p port -h machine" << std::endl;
    std::cout << "\t-p port \tPort number" << std::endl;
    std::cout << "\t-h machine \tHostname of the server" << std::endl;
}
