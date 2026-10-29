# RPG Inventory Server Backend

<div align="center">

  <img src="https://img.shields.io/badge/C%2B%2B-98.7%25-00599C?style=for-the-badge&logo=c%2B%2B" alt="C++" />
  <img src="https://img.shields.io/badge/CMake-1.3%25-064F8C?style=for-the-badge&logo=cmake" alt="CMake" />

  <h3>A multiplayer RPG inventory management system built in C++</h3>

  <p>
    A compact backend simulation for managing player inventories, random loot generation,
    item rarity progression, trade logic, and item ownership tracking in a fantasy RPG setting.
  </p>

</div>

## Overview

This project models a lightweight inventory backend for a multiplayer RPG. It is built around a nested data structure:

- outer container: players
- inner container: each player's inventory
- map key: unique item ID
- value: list of item properties

In other words, the system uses:

```cpp
using Inventory = map<int, list<string>>;
using Players = list<Inventory>;
```

This design allows:

- multiple players to have separate inventories
- each item to have a unique ID
- item metadata to be stored as a list of strings
- efficient lookups by unique item ID
- simple implementation of trading and removal logic

---

## Key Features

### 1. Player-based inventory management
Each player is represented as an independent inventory map. The system maintains a list of all players, making it easy to iterate over them and manage their contents.

### 2. Unique item tracking
Every item inserted into the system gets a unique integer ID, ensuring that each object can be referenced individually.

### 3. Randomized RPG loot system
The project includes a curated database of fantasy-style items divided into seven rarity tiers:

- Common
- Uncommon
- Rare
- Epic
- Legendary
- Mythic
- Artifact

Items include standard equipment such as swords, armor, potions, and magical accessories, as well as humorous meme-tier objects like:

- Rubber Chicken
- Bonk Bat
- The "This is Fine" Mug
- Infinite Loop Hourglass
- Plot Armor
- The Ban Hammer

### 4. Enchantment generation
Loot may receive an enchantment string from a list of bonuses such as:

- +10% Attack Speed
- +5% Critical Hit Chance
- Life Steal: 3%
- Movement Speed: +15
- Mana Regeneration: +2.5/s
- 15% Chance to Stun
- Armor Penetration: +10

### 5. Trading / inventory transfer
The system supports transferring an item from one player inventory to another using a `swapItem()` method. This makes it usable as a simple backend for multiplayer inventory exchange.

### 6. Item deletion and lookup
Items can be removed by unique ID, and the system reports whether the item was found and removed or whether it does not exist.

### 7. Display utilities
The project includes formatting helpers that print inventory contents in a readable, debug-friendly format.

---

## Data Model

The backend is intentionally simple and educational. The logical structure is:

```text
Players
└── Inventory for Player 0
    ├── {1000, ["Sword", "Weapon", "Rare", "+10% Attack Speed"]}
    ├── {1001, ["Mana Potion", "Potion", "Epic"]}
    └── {1002, ["Cloak of Shadows", "Armor", "Legendary"]}

└── Inventory for Player 1
    └── ...
```

The actual definition is:

```cpp
using Inventory = map<int, list<string>>;
using Players = list<Inventory>;
```

### Item property format
Each item is stored as a list of strings, typically in this pattern:

```cpp
{"Item Name", "Weapon", "Epic", "+10% Attack Speed"}
```

Property list meaning:

1. Base item name
2. Item category (`Weapon`, `Armor`, `Potion`, `Accessory`)
3. Rarity tier (`Common`, `Uncommon`, `Rare`, ...)
4. Optional enchantment text

---

## Core System Design

### `InventoryManager`
The central class is `InventoryManager`, which encapsulates:

- the list of all players
- the next available item ID
- total item count

It provides the operational API:

- `addItem(int player_idx, list<string> properties)`
- `removeItem(int unique_item_idx)`
- `swapItem(int p_idx_from, int p_idx_to, int unique_item_idx)`
- `LOOTING(int player_idx, int loot_amount = 1)`
- `showPlayerInventory(int player_idx)`
- `showALL_INVENTORY()`
- `Get_totalItems()`

### `addItem()`
Adds an item to a specific player inventory and assigns it a unique ID. This function also prints a human-readable line indicating the item gained.

### `removeItem()`
Searches through all player inventories to find the item by its unique ID and removes it.

### `swapItem()`
Transfers an item from one player to another while preserving the item properties.

### `LOOTING()`
This method selects a random item from the base loot database, rolls a rarity modification, optionally adds an enchantment, and inserts the item into the selected player's inventory.

---

## Loot and Rarity System

The project contains a large `base_items` list containing fantasy and meme-style weapons, armor, accessories, and potions. Each item entry follows this structure:

```cpp
{"Item Name", "Category", rarityTier}
```

Example entries:

```cpp
{"Dragon-Tooth Blade", "Weapon", 4},
{"Sun-Blessed Aegis", "Armor", 4},
{"Ambrosia of the Gods", "Potion", 5},
{"Plot Armor", "Armor", 6},
{"The Ban Hammer", "Weapon", 6}
```

The rarity mapping is:

```cpp
list<string> rarities = {
    "Common",
    "Uncommon",
    "Rare",
    "Epic",
    "Legendary",
    "Mythic",
    "Artifact"
};
```

The code randomly upgrades an item's tier within a bounded range, giving it a more powerful drop without making every item static.

---

## Example Runtime Flow

The `main()` function demonstrates the intended usage of the backend:

1. generate a random number of players
2. add items to different inventories
3. loot random items for a random player
4. remove an item by ID
5. display one or more inventories
6. show all inventories
7. transfer an item between players

This makes the project ideal as a classroom demonstration of:

- STL containers
- custom data structures
- object-oriented design
- random generation
- simple game systems simulation

---

## Repository Structure

```text
RPG-Inventory-Server_BackEnd/
├── CMakeLists.txt
├── main.cpp
├── adatszerkezet_gen_kod.cpp
├── .idea/
└── README.md
```

### Files

#### `main.cpp`
This is the main implementation of the RPG inventory system. It contains:

- item database
- rarity/enchantment definitions
- overloads for stream output
- the `InventoryManager` class
- the demo `main()` program

#### `adatszerkezet_gen_kod.cpp`
This file is a helper utility that demonstrates how nested containers such as:

```cpp
list<map<int, list<string>>>
```

can be generated algorithmically from a string hash. It is not the main gameplay backend, but it is related to the structural design of the inventory system.

#### `CMakeLists.txt`
Used to configure the project and build the executable named `Beadando_2`.

---

## Building the Project

This project uses CMake.

### Linux / macOS

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
./Beadando_2
```

### Windows (with CMake + MSBuild / Visual Studio)

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Release
.
```

Then run the resulting executable from the build directory.

---

## Requirements

- CMake 3+ (the project declares `cmake_minimum_required(VERSION 4.1)` in `CMakeLists.txt`)
- Modern C++ compiler
- Support for C++26 (`CMAKE_CXX_STANDARD 26`)

> Note: Because the project sets `CMAKE_CXX_STANDARD 26`, a recent compiler is strongly recommended.

---

## Example Output

The program prints inventory events such as:

```text
Player ID: 0 gained {Item ID: 1000}, Weapon: Vaskard [Common]
Player ID: 2 gained {Item ID: 1001}, Armor: Chainmail Hauberk [Rare] with +10% Attack Speed
Item with id: 1003 was removed from its player's inventory.
Inventory of the Player with index: 2:
{Item ID: 1004}, Potion: Ambrosia of the Gods [Mythic]
```

This output is intentionally simple and suitable for assignment/demo purposes.

---

## Why This Project Is Interesting

This project is a good example of using STL containers to model a game ecosystem without a database or external game engine. It shows how:

- `list`, `map`, and nested container structures can represent player state
- random content generation can be used to simulate loot drops
- object IDs create reliable references for game entities
- a simple C++ backend can form the basis for larger systems

It is especially useful as:

- a learning project for data structures
- a small RPG systems prototype
- a C++ assignment demonstrating container usage

---

## Notes

This project is a compact, assignment-style implementation rather than a production-grade game backend. It is intentionally lightweight and focuses on demonstrating how data structures and game logic can work together in C++.

The system includes humorous meme items alongside serious RPG gear, which makes the project memorable while still showing real backend logic.

---

## Future Enhancements

Possible improvements for a more advanced version of this project include:

- persistent storage to save player inventories to disk
- a proper `Item` class instead of `list<string>` metadata
- structured stats for damage, defense, health, and mana
- sorting/filtering by rarity or type
- multiplayer networking layer
- proper JSON or database-backed persistence

---

## Summary

`RPG Inventory Server Backend` is a compact, fantasy-themed C++ inventory system built around nested STL containers. It manages player inventories, item IDs, loot generation, rarity tiers, enchantments, and simple item trade operations. The project is a solid example of how a small backend can simulate game inventory logic in a clean, educational way.

If you are using this project as a learning exercise or a portfolio piece, it demonstrates strong fundamentals in:

- C++ data structure design
- random game systems
- backend workflow modeling
- educational software engineering

Enjoy the loot.

---

<div align="center">
  <p><strong>Built with C++ • CMake • RPG-inspired game logic</strong></p>
</div>
