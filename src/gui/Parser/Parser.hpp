/*
** EPITECH PROJECT, 2025
** B-YEP-400-STG-4-1-zappy-arthur.desjonqueres
** File description:
** Parser
*/

#ifndef PARSER_HPP_
    #define PARSER_HPP_

#include <exception>
#include <string>
#include <iostream>

namespace Zappy {
    /// @brief Struct to be returned by the parsing of arguments
    struct Config {
        std::string ip;
        int port;
    };

    /// @brief Error class to handle parsing errors;
    class ParsingError : public std::exception {
        public:
            explicit ParsingError(const std::string &msg);
            const char *what() const noexcept override;

        private:
            std::string _message;
    };

    class Parser {
        public:
            Parser() = delete;
            ~Parser() = default;

            /// @brief Retrieves the port and ip from the arguments
            /// @param argc The number of arguments
            /// @param argv The arguments
            /// @return A config struct filled with the ip and the port
            static Config parseArguments(const int argc, const char **argv);
            /// @brief Prints how the program is supposed to be used
            static void printUsage();
        protected:
        private:
    };
}

#endif /* !PARSER_HPP_ */
