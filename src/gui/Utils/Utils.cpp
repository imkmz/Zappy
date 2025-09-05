/*
** EPITECH PROJECT, 2025
** B-CCP-400-STG-4-1-theplazza-mikaela.rabines
** File description:
** Utils
*/

#include "Utils.hpp"

std::string Zappy::Utils::cleanStr(const std::string &input)
{
    std::string cleaned;

    std::unique_copy(input.begin(), input.end(),
        std::back_insert_iterator<std::string>(cleaned),
        [](char a, char b) {return isspace(a) && isspace(b);}
    );
    while (isspace(cleaned[0]))
        cleaned.erase(0, 1);
    while (isspace(cleaned[cleaned.length() - 1]))
        cleaned.erase(cleaned.length() - 1, cleaned.length());
    for (char &c : cleaned)
        if (isspace(c))
            c = ' ';
    return cleaned;
}

std::vector<std::string> Zappy::Utils::split(const std::string &input, char delim)
{
    std::vector<std::string> arr;
    size_t pos = 0;
    size_t last = 0;

    while((pos = input.find(delim, last)) != std::string::npos) {
        arr.push_back(input.substr(last, pos - last));
        last = pos + 1;
    }
    arr.push_back(input.substr(last, pos - last));
    arr.shrink_to_fit();
    return arr;
}
