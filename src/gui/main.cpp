/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** main
*/

#include "Parser/Parser.hpp"
#include "NetworkUtils/NetworkUtils.hpp"
#include "Display/Display.hpp"
#include "Cache/Cache.hpp"
#include <cstdlib>
#include <ctime>
#include <memory>

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    bool connectionSuccess = false;

    srand(time(nullptr));
    try
    {
        Zappy::Config conf = Zappy::Parser::parseArguments(argc, argv);
        std::shared_ptr<Zappy::INetwork> network = std::make_shared<Zappy::NetworkUtils>(conf.ip, conf.port);

        network->tryConnect();

        std::cout << "Connected to server" << std::endl;
        connectionSuccess = true;

        SetTraceLogLevel(LOG_ERROR);

        Zappy::Display display(25, 25, network);
        display.displayLoop();
    }
    catch (const Zappy::ParsingError &err)
    {
        std::cerr << "Arguments error : " << err.what() << std::endl;
        Zappy::Parser::printUsage();
        return 84;
    }
    catch (const Zappy::NetworkError &err)
    {
        std::cerr << "Connection Error : " << err.what() << std::endl;
        if (!connectionSuccess)
            return 84;
    }
    return 0;
}