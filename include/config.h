/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** config
*/

#ifndef CONFIG_H_
    #define CONFIG_H_

    // param pour le game
    #define MAX_PLAYERS 100
    #define MAX_TEAMS 10
    #define MAX_TEAM_NAME 50
    #define MAX_CLIENTS_PER_TEAM 50
    #define MAX_PENDING_COMMANDS 10
    #define DEFAULT_FREQ 100

    // les temps d'executions
    #define TIME_FORWARD 7
    #define TIME_RIGHT 7
    #define TIME_LEFT 7
    #define TIME_LOOK 7
    #define TIME_INVENTORY 1
    #define TIME_BROADCAST 7
    #define TIME_FORK 42
    #define TIME_EJECT 7
    #define TIME_TAKE 7
    #define TIME_SET 7
    #define TIME_INCANTATION 300

    // param pour la survie
    #define INITIAL_LIFE_UNITS 10
    #define LIFE_UNIT_DURATION 126

    // densité des res
    #define DENSITY_FOOD 0.5
    #define DENSITY_LINEMATE 0.3
    #define DENSITY_DERAUMERE 0.15
    #define DENSITY_SIBUR 0.1
    #define DENSITY_MENDIANE 0.1
    #define DENSITY_PHIRAS 0.08
    #define DENSITY_THYSTAME 0.05

#endif /* !CONFIG_H_ */
