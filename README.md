# Zappy GUI

The **Zappy GUI** is a graphical client for the Zappy project, providing a real-time 3D and 2D visualization of the game world, player actions, teams, and resources. It is built using C++17 and leverages [raylib-cpp](https://github.com/RobLoach/raylib-cpp) for rendering, input, and audio. The GUI is designed to interact with the Zappy server, displaying the state of the game and allowing users to observe the simulation in detail.

---

## Table of Contents

- [Features](#features)
- [Architecture Overview](#architecture-overview)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Main Components](#main-components)
- [Networking](#networking)
- [Extending the GUI](#extending-the-gui)
- [Troubleshooting](#troubleshooting)
- [License](#license)

---

## Features

- **3D Map Rendering:** Visualizes the game map, including tiles, resources, and eggs.
- **Player Visualization:** Shows players with orientation, level, team, and inventory.
- **Team Management:** Displays teams, their players, and eggs.
- **Resource Display:** Renders resources on tiles and updates them in real-time.
- **Tooltip System:** Hover or click on entities to display detailed tooltips.
- **Scoreboard:** Press TAB to view team and player statistics.
- **Broadcast Messages:** Displays server broadcast messages in the GUI.
- **Camera Controls:** Move and rotate the camera to explore the map.
- **Network Integration:** Connects to the Zappy server, parses protocol messages, and updates the world accordingly.
- **Animations:** Supports player animations and incantation effects.

---

## Architecture Overview

The GUI is modular and object-oriented, with a clear separation between rendering, game logic, and networking. Key interfaces such as ```IClickable```, ```IDrawable```, and ```ICache``` allow for extensibility and polymorphism.

- **Rendering:** Uses raylib-cpp for 3D/2D drawing.
- **Game State:** Maintains a local representation of the world, updated via network messages.
- **Input Handling:** Processes mouse and keyboard events for interaction and camera control.
- **Networking:** Receives and parses messages from the Zappy server asynchronously.

---

## Build Instructions

### Prerequisites

- C++17 compatible compiler (e.g., g++)
- [raylib](https://www.raylib.com/) and [raylib-cpp](https://github.com/RobLoach/raylib-cpp)
- Make

### Building

```sh
make zappy_gui
```

This will compile the GUI and place the executable in the project root.

---

## Usage

```sh
./zappy_gui -p <port> -h <host>
```

- `-p <port>`: Port to connect to the Zappy server.
- `-h <host>`: Hostname or IP address of the Zappy server.

**Controls:**
- **Left Click:** Select an entity and show its tooltip.
- **Right Click/Arrow keys:** Rotates the camera.
- **ZQSD/WASD:** Moves the camera
- **TAB:** Show/hide the scoreboard.
- **Keypad + / -:** Adjust game frequency.
- **ESC:** Quit the GUI.

---

## Main Components

### 1. Display

- **[Display](src/gui/Display/Display.cpp):** Main application class. Handles the render loop, input, and delegates to other components.

### 2. Map and Tiles

- **[Map](src/gui/Map/Map.cpp):** Manages the grid of [Tile](src/gui/Tile/Tile.cpp) objects.
- **[Tile](src/gui/Tile/Tile.cpp):** Represents a single map tile, including resources and selection state.

### 3. Players and Eggs

- **[Player](src/gui/Player/Player.cpp):** Represents a player, including position, orientation, team, inventory, and animations.
- **[Egg](src/gui/Egg/Egg.cpp):** Represents an egg entity on the map.

### 4. Teams

- **[Team](src/gui/Team/Team.cpp):** Holds players and eggs for a team.
- **[TeamManager](src/gui/TeamManager/TeamManager.cpp):** Manages all teams.

### 5. UI

- **[Tooltip](src/gui/Tooltip/Tooltip.cpp):** Displays information about the selected entity.
- **[Scoreboard](src/gui/Scoreboard/Scoreboard.cpp):** Shows team and player stats.

### 6. Networking

- **[NetworkUtils](src/gui/NetworkUtils/NetworkUtils.cpp):** Handles communication with the server, parses protocol messages, and updates the game state.

### 7. Rendering and Resources

- **[Cache](src/gui/Cache/Cache.cpp):** Loads and caches models, textures, and colors for efficient rendering.
- **[Raylib](src/gui/Raylib/):** C++ wrappers for raylib types (models, textures, audio, etc.).

---

## Networking

The GUI connects to the Zappy server using TCP. It receives protocol messages, parses them, and updates the local game state accordingly. The networking is handled in a separate thread to keep the UI responsive.

- **Supported Protocol Commands:** Map size, player actions, resource updates, broadcasts, etc.
- **Extensible Command Handling:** New protocol commands can be added in [NetworkUtils](src/gui/NetworkUtils/NetworkUtils.cpp).

---

## Extending the GUI

- **Add New Entities:** Implement ```IDrawable``` and/or ```IClickable``` interfaces for new game objects.
- **Custom Animations:** Extend [AnimManager](src/gui/AnimManager/AnimManager.cpp) for new animation types.
- **UI Enhancements:** Add new UI components by extending the rendering in [Display](src/gui/Display/Display.cpp) or creating new classes.
- **Protocol Extensions:** Add new command handlers in [NetworkUtils](src/gui/NetworkUtils/NetworkUtils.cpp).

---

## Troubleshooting

- **Black Screen or Crash:** Ensure all required models and textures are present in the ```ressources/``` directory.
- **Network Errors:** Check that the server is running and accessible at the specified host and port.

---

## License

This project uses [raylib-cpp](https://github.com/RobLoach/raylib-cpp) under the zlib/libpng license.
See the main project [LICENSE](LICENSE) for details.

---

## Credits

- [raylib](https://www.raylib.com/)
- [raylib-cpp](https://github.com/RobLoach/raylib-cpp)
- Epitech Zappy Project Contributors

---

*For more information, see the source code in [src/gui/](src/gui/) and the project documentation (README.md).*

## Thanks

Made with Thibaud (GUI), Arthur (GUI), Aurélien (Network), Alexandre (Network), Pierre-Louis (IA), and myself (IA).
