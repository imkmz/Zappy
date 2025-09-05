/*
** EPITECH PROJECT, 2025
** B-CCP-400-STG-4-1-theplazza-mikaela.rabines
** File description:
** Utils
*/

#ifndef UTILS_HPP_
    #define UTILS_HPP_

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>

namespace Zappy {
    class Utils {
        public:
            Utils() = delete;
            ~Utils() = delete;

            /// @brief Removes all superfluous white space from a string
            /// @param input The string to clean
            /// @return The cleaned string
            static std::string cleanStr(const std::string &input);
            /// @brief Splits a string into a vector of string, separating the words by spaces
            /// @param input The string to split
            /// @param delim The delimiter to split the string at
            /// @return The vector of strings
            static std::vector<std::string> split(const std::string &input, char delim);
    };
}

#endif /* !UTILS_HPP_ */
