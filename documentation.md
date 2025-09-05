# Documentation Projet Zappy

## Table des Matières
1. [Vue d'ensemble](#vue-densemble)
2. [Architecture du projet](#architecture-du-projet)
3. [Serveur (zappy_server)](#serveur-zappy_server)
4. [Interface Graphique (zappy_gui)](#interface-graphique-zappy_gui)
5. [Intelligence Artificielle (zappy_ai)](#intelligence-artificielle-zappy_ai)
6. [Protocole de Communication](#protocole-de-communication)
7. [Compilation et Utilisation](#compilation-et-utilisation)
8. [Structures de Données](#structures-de-données)

## Vue d'ensemble

Zappy est un projet de jeu en réseau multijoueur où plusieurs équipes s'affrontent sur une carte contenant des ressources. L'objectif est d'être la première équipe à avoir au moins 6 joueurs atteignant le niveau maximum (niveau 8).

### Composants principaux
- **Serveur** (C) : Gère la logique du jeu et les communications
- **Interface Graphique** (C++) : Visualisation en temps réel du monde
- **Intelligence Artificielle** (Python) : Clients autonomes contrôlant les joueurs

## Architecture du projet

```
zappy/
├── src/
│   ├── server/           # Code du serveur en C
│   ├── gui/              # Interface graphique en C++
│   └── ai/               # Intelligence artificielle en Python
├── includes/             # Fichiers d'en-tête
├── obj/                  # Fichiers objets (générés)
├── Makefile
├── zappy_server          # Exécutable serveur
├── zappy_gui             # Exécutable GUI
└── zappy_ai              # Script Python AI
```

## Serveur (zappy_server)

### Fonctionnalités principales

Le serveur est écrit en C et gère tous les aspects du jeu :
- Gestion des connexions réseau avec `poll()`
- Monde de jeu toroïdal avec ressources
- Gestion des équipes et joueurs
- Système d'élévation avec rituels d'incantation
- Communication avec l'interface graphique

### Structures de données clés

#### Configuration du serveur
```c
typedef struct {
    int port;                           // Port d'écoute
    int width, height;                  // Dimensions du monde
    char teams[MAX_TEAMS][MAX_TEAM_NAME]; // Noms des équipes
    int team_count;                     // Nombre d'équipes
    int clients_per_team;               // Clients autorisés par équipe
    int freq;                           // Fréquence de jeu
} server_config_t;
```

#### État du jeu
```c
typedef struct game_s {
    map_t map;                          // Carte du monde
    team_t teams[MAX_TEAMS];            // Équipes
    player_t players[MAX_PLAYERS];      // Joueurs
    incantation_t *incantations;        // Rituels en cours
    int time_unit;                      // Unité de temps
    int freq;                           // Fréquence
    // ... autres champs
} game_t;
```

### Gestion des commandes

Le serveur utilise un système de file d'attente pour gérer les commandes des joueurs :

```c
typedef struct command_queue_s {
    pending_command_t *head;
    pending_command_t *tail;
    int count;
} command_queue_t;
```

### Commandes supportées

| Commande | Temps | Description |
|----------|-------|-------------|
| `Forward` | 7/f | Avancer d'une case |
| `Right` | 7/f | Tourner à droite |
| `Left` | 7/f | Tourner à gauche |
| `Look` | 7/f | Observer autour |
| `Inventory` | 1/f | Voir l'inventaire |
| `Broadcast` | 7/f | Diffuser un message |
| `Connect_nbr` | - | Nombre de connexions |
| `Fork` | 42/f | Pondre un œuf |
| `Eject` | 7/f | Éjecter les joueurs |
| `Take` | 7/f | Prendre un objet |
| `Set` | 7/f | Poser un objet |
| `Incantation` | 300/f | Commencer l'élévation |

### Système d'élévation

Les joueurs peuvent s'élever en niveau selon des critères précis :

| Niveau | Joueurs requis | linemate | deraumere | sibur | mendiane | phiras | thystame |
|--------|----------------|----------|-----------|-------|----------|---------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

### Utilisation
```bash
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 6 -f 100
```

## Interface Graphique (zappy_gui)

### Fonctionnalités

L'interface graphique est développée en C++ avec Raylib et offre :
- Visualisation 2D/3D du monde de jeu
- Affichage en temps réel des joueurs et ressources
- Suivi des statistiques des équipes
- Communication avec le serveur via le protocole GUI

### Architecture

```cpp
class Display {
    // Gestion de l'affichage principal
    void displayLoop();
    void render2D();
    void render3D();
};

class NetworkUtils : public INetwork {
    // Communication réseau
    void tryConnect();
    void sendCommand(const std::string& cmd);
};

class Cache {
    // Mise en cache des données du serveur
    void updateGameState();
};
```

### Authentification

L'interface graphique s'authentifie auprès du serveur avec le nom d'équipe spécial `"GRAPHIC"`.

### Utilisation
```bash
./zappy_gui -p 4242 -h localhost
```

## Intelligence Artificielle (zappy_ai)

### Stratégie

L'IA est implémentée en Python et utilise une approche basée sur les états :

```python
class AILogic:
    def __init__(self):
        self.level = 1
        self.inventory = {}
        self.position = (0, 0)
        self.command_queue = []
        
    def decide_next_action(self):
        # Logique de décision basée sur l'état actuel
        if self.need_food():
            self.search_food()
        elif self.can_level_up():
            self.prepare_incantation()
        else:
            self.explore()
```

### Phases de comportement

1. **Exploration** : Recherche de ressources sur la carte
2. **Collecte** : Ramassage des objets nécessaires
3. **Élévation** : Préparation et exécution des rituels
4. **Reproduction** : Création de nouveaux joueurs (Fork)

### Gestion de la survie

- Surveillance constante des unités de vie
- Recherche prioritaire de nourriture quand nécessaire
- Optimisation des déplacements

### Utilisation
```bash
./zappy_ai -p 4242 -n team1 -h localhost
```

## Protocole de Communication

### Client → Serveur (AI)

1. **Connexion initiale**
   ```
   <-- WELCOME\n
   --> TEAM_NAME\n
   <-- CLIENT_NUM\n
   <-- X Y\n
   ```

2. **Commandes de jeu**
   ```
   --> Forward\n
   <-- ok\n
   
   --> Look\n
   <-- [player,food,linemate,...]\n
   
   --> Inventory\n
   <-- [food 5, linemate 2, ...]\n
   ```

### Serveur → GUI

Le serveur envoie des événements spécifiques à l'interface graphique :

- `msz X Y` : Taille de la carte
- `bct X Y q0 q1 q2 q3 q4 q5 q6` : Contenu d'une case
- `pnw n X Y o l N` : Nouveau joueur
- `ppo n X Y o` : Position d'un joueur
- `plv n l` : Niveau d'un joueur
- `pin n X Y q0 q1 q2 q3 q4 q5 q6` : Inventaire d'un joueur

### Messages de diffusion

```
Broadcast message\n
→ message K,text\n  (K = direction du son)
```

## Compilation et Utilisation

### Compilation

```bash
make                    # Compile tous les composants
make zappy_server      # Compile uniquement le serveur
make zappy_gui         # Compile uniquement la GUI
make zappy_ai          # Prépare l'exécutable AI
```

### Nettoyage

```bash
make clean             # Supprime les fichiers objets
make fclean           # Supprime tout
make re               # Recompile tout
```

### Vérification du style

```bash
make cs               # Vérifie le coding style EPITECH
```

### Exemple d'utilisation complète

1. **Démarrer le serveur**
   ```bash
   ./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 team3 -c 10 -f 100
   ```

2. **Lancer l'interface graphique**
   ```bash
   ./zappy_gui -p 4242 -h localhost
   ```

3. **Connecter des IA**
   ```bash
   ./zappy_ai -p 4242 -n team1 -h localhost &
   ./zappy_ai -p 4242 -n team2 -h localhost &
   ./zappy_ai -p 4242 -n team3 -h localhost &
   ```

## Structures de Données

### Position et Inventaire

```c
typedef struct {
    int x, y;
} position_t;

typedef struct {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;
```

### Joueur

```c
typedef struct {
    inventory_t inventory;
    position_t pos;
    int id;
    int last_eat;
    char team_name[50];
    int level;
    int socket_fd;
    int orientation;
    int is_in_incantation;
    int dead;
} player_t;
```

### Carte

```c
typedef struct {
    int width;
    int height;
    inventory_t **tiles;    // Tableau 2D des ressources par case
} map_t;
```

### Densité des ressources

| Ressource | Densité |
|-----------|---------|
| food | 0.5 |
| linemate | 0.3 |
| deraumere | 0.15 |
| sibur | 0.1 |
| mendiane | 0.1 |
| phiras | 0.08 |
| thystame | 0.05 |

### Configuration par défaut

- **Fréquence** : 100 Hz
- **Unités de vie initiales** : 10 (= 1260 unités de temps)
- **Durée d'une unité de vie** : 126 unités de temps
- **Monde toroïdal** : Les bords se rejoignent

## Notes techniques

### Gestion mémoire
- Le serveur utilise `malloc/free` pour l'allocation dynamique
- L'interface graphique utilise RAII en C++
- L'IA Python utilise le garbage collector automatique

### Concurrence
- Le serveur est mono-thread avec `poll()` pour la multiplexation
- Évite l'attente active (busy waiting)
- Gestion des signaux pour l'arrêt propre

### Portabilité
- Compatible Linux/Unix
- Utilise des sockets TCP standard
- Interface graphique avec Raylib (multiplateforme)

---

Cette documentation couvre l'architecture complète du projet Zappy. Pour plus de détails sur l'implémentation, consultez les fichiers source dans les répertoires respectifs.