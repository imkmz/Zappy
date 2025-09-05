/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** INetwork
*/

#ifndef INETWORKUTILS_HPP_
    #define INETWORKUTILS_HPP_

#include <string>
#include <vector>
#include <memory>
#include <optional>

#include "TeamManager/TeamManager.hpp"
#include "Map/Map.hpp"

namespace Zappy {
    class INetwork {
    public:
        virtual ~INetwork() = default;

        virtual void tryConnect() = 0;
        virtual std::string receiveMessage() = 0;
        virtual int getFd() const = 0;
        virtual int getTimeUnit() const = 0;
        virtual bool validateCommand(const std::vector<std::string> &cmd) const = 0;
        virtual bool execCommand() = 0;
        virtual void sendToServer(const std::string &str) = 0;
        virtual std::string getBroadcast() const = 0;
        virtual void setTeamManager(std::shared_ptr<TeamManager> tm) = 0;
        virtual void setMap(std::shared_ptr<Map> map) = 0;
        virtual void changeTimeUnit(int unit) = 0;
        virtual std::optional<std::tuple<raylib::Vector2, int, int, int, std::string>> popPlyRequest() = 0;
    };
}

#endif // INETWORKUTILS_HPP_
