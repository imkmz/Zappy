/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** NetworkUtils
*/

#ifndef NETWORKUTILS_HPP_
    #define NETWORKUTILS_HPP_

#include <string>
#include <arpa/inet.h>
#include <variant>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <queue>
#include <sstream>
#include <mutex>

#include "Utils/Utils.hpp"
#include "Map/Map.hpp"
#include "TeamManager/TeamManager.hpp"
#include "INetwork/INetwork.hpp"

namespace Zappy {

    /// @brief Error class to handle networking errors
    class NetworkError : public std::exception {
        public:
            explicit NetworkError(const std::string &msg);
            const char *what() const noexcept override;

        private:
            std::string _message;
    };

    enum Symbol {
        X,
        Y,
        q0,
        q1,
        q2,
        q3,
        q4,
        q5,
        q6,
        n,
        O,
        L,
        e,
        T,
        N,
        mN,
        R,
        M,
        i
    };

    using SymbolsArguments = std::vector<Symbol>;

    class NetworkUtils : public INetwork {
        public:
            /// @brief Constructor of the networking wrapper, stores the ip and port in an usable way
            /// @param ip Ip given as an argument at the start of the program
            /// @param port Port given as an argument at the start of the program
            NetworkUtils(const std::string &ip, const int port);
            ~NetworkUtils() = default;

            /// @brief Try to connect using the ip and port given in the constructor
            void tryConnect() override;
            /// @brief Receives a mess$age from a socket
            /// @return The message received
            std::string receiveMessage() override;
            /// @brief Gets the server's fd
            /// @return Server's fd
            int getFd() const override;
            /// @brief Checks if the command is valid using its arguments
            /// @param cmd The command
            /// @return If its valid or not
            bool validateCommand(const std::vector<std::string> &cmd) const override;
            /// @brief Exececute the first element of the command queue
            /// @return If the command is valid or not
            bool execCommand() override;
            /// @brief Send message to server
            /// @param str String to send
            void sendToServer(const std::string &str) override;
            void changeTimeUnit(int unit) override;
            int getTimeUnit() const override;
            std::optional<std::tuple<raylib::Vector2, int, int, int, std::string>> popPlyRequest() override;

            /// @brief Gets the broadcast string which is a global message sent by the server
            /// @return The string broadcasted by the server
            std::string getBroadcast() const override;

            /// @brief Assigns a teamManager to the networking system
            /// @param tm The teamManager to assign
            void setTeamManager(std::shared_ptr<TeamManager> tm) override;
            void setMap(std::shared_ptr<Map> map) override;
        private:
            in_addr_t _ip;
            int _port;
            int _fd;
            int _timeUnit;
            bool _waitForTimeChange = false;
            raylib::Vector2 _mapSize;

            int _bctCount = 0;

            std::string _broadcast;

            std::shared_ptr<Map> _map;
            std::unordered_map<int, Resources> _ressourceNbMap;
            std::unordered_map<Symbol, std::function<bool(const std::string&)>> _symbolValidity;
            std::unordered_map<std::string, SymbolsArguments> _commands;
            std::queue<std::vector<std::string>> _cmdQueue;
            std::shared_ptr<TeamManager> _teamManager;
            std::shared_ptr<std::mutex> _plyMutex;
            std::queue<std::tuple<raylib::Vector2, int, int, int, std::string>> _plyRequest;
            std::unordered_map<Symbol, std::function<bool(const std::string&)>> genSymbolValidity() const;
            std::unordered_map<std::string, SymbolsArguments> genCommands() const;

            void msz(const std::vector<std::string>& args);
            void bct(const std::vector<std::string>& args);
            void tna(const std::vector<std::string>& args);
            void pnw(const std::vector<std::string>& args);
            void ppo(const std::vector<std::string>& args);
            void plv(const std::vector<std::string>& args);
            void pin(const std::vector<std::string>& args);
            void pex(const std::vector<std::string>& args);
            void pbc(const std::vector<std::string>& args);
            void pic(const std::vector<std::string>& args);
            void pie(const std::vector<std::string>& args);
            void pfk(const std::vector<std::string>& args);
            void pdr(const std::vector<std::string>& args);
            void pgt(const std::vector<std::string>& args);
            void pdi(const std::vector<std::string>& args);
            void enw(const std::vector<std::string>& args);
            void ebo(const std::vector<std::string>& args);
            void edi(const std::vector<std::string>& args);
            void sgt(const std::vector<std::string>& args);
            void sst(const std::vector<std::string>& args);
            void seg(const std::vector<std::string>& args);
            void smg(const std::vector<std::string>& args);
            void suc(const std::vector<std::string>& args);
            void sbp(const std::vector<std::string>& args);
            void welcome(const std::vector<std::string>& args);
    };
}

#endif /* !NETWORKUTILS_HPP_ */
