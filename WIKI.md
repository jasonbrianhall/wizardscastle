# Welcome to the Wizard's Castle Wiki!

This wiki serves as a comprehensive guide to the Wizard's Castle game, its mechanics, and development. Whether you're a player looking to master the game or a developer interested in contributing, you'll find valuable information here.

## Table of Contents

1. [Game Overview](#game-overview)
2. [Gameplay Mechanics](#gameplay-mechanics)
3. [Characters and Races](#characters-and-races)
4. [Items and Treasures](#items-and-treasures)
5. [Monsters and Combat](#monsters-and-combat)
6. [Magic and Spells](#magic-and-spells)
7. [Castle Layout](#castle-layout)
8. [Development Guide](#development-guide)
9. [Version History](#version-history)
10. [FAQs](#faqs)

## Game Overview

Wizard's Castle is a text-based roguelike adventure game where players explore a randomly generated castle in search of the legendary Orb of Zot. The game combines elements of strategy, resource management, and chance as players navigate through various rooms, battle monsters, and collect treasures. Originally conceived in 1980 by Joseph R. Power, this implementation brings the classic game to modern systems.

## Gameplay Mechanics

### Basic Controls

- Use single-letter commands to interact with the game (with the exception being drink):
  - N/S/E/W: Move in cardinal directions
  - U/D: Go up or down stairs
  - DR: Drink from a pool
  - M: Display map of current level
  - F: Light a flare
  - L: Use lamp to look into adjacent room
  - O: Open a chest or book
  - G: Gaze into a crystal orb
  - T: Teleport (requires Runestaff)
  - Q: Quit the game
  - Z: Display player status

### Turn System

Each action you take counts as a turn. Monsters are statically located, and certain events (like curses) may occur randomly between turns.

### Exploring the Castle

The castle consists of 8 levels, each with 64 rooms (8x8 grid). You start at the entrance and must find your way through the castle, discovering rooms as you go. Use the 'M' command to view a map of discovered rooms on your current level.

### Resource Management

Manage your gold, flares, armor durability, and health (strength) carefully. You can buy supplies from vendors or find them in the castle.

## Characters and Races

### Character Creation

At the start of the game, you'll choose your race and allocate attribute points.

### Race Attributes

Choose from four races, each with unique starting attributes:
- Hobbit
- Elf
- Human
- Dwarf
- Dark Elf (Drow)

### Leveling and Progression

There's no traditional leveling system. Improve your character by finding magical pools, purchasing attribute enhancements from vendors, or finding powerful items.

## Items and Treasures

### Weapons and Armor

Weapons:
- Dagger
- Mace
- Sword
- Excalibur (not purchasable)

Armor:
- Leather
- Chainmail
- Plate
- Stone (not purchasable)

### Magical Items

- Lamp: Allows you to peek into adjacent rooms
- Flares: Reveal all adjacent rooms
- Runestaff: Allows teleportation
- Orb of Zot: The ultimate goal of the game

### Treasures and Their Effects

- Ruby Red: Helps avoid lethargy
- Pale Pearl: Protects from leeches
- Green Gem: Helps avoid forgetting
- Opal Eye: Cures blindness
- Blue Flame: Dissolves sticky books
- Norn Stone, Palantir, Silmaril: Valuable but no special effects

## Monsters and Combat

### Monster Types

Enemies range from weak to strong:
- Kobold, Orc, Wolf, Goblin, Ogre, Troll, Bear, Minotaur, Gargoyle, Chimera, Balrog, Dragon
- Gargoyles and Dragons can break weapons so be careful
- You can not attack monsters with your bare hands or if you have a book stuck to your hand
- Some enemies have magical powers like Kobolds, Balrogs, and Dragons

### Combat System

Combat is turn-based. You can choose to attack, retreat, attempt to bribe, or cast a spell (if your intelligence is high enough).  Species may have unique attacks which will change as the game is developed.

## Magic and Spells

### Spell Types

- Web: Immobilizes the enemy
- Fireball: Deals damage
- Deathspell: Instantly kills the enemy (or you, if it fails)
- Heal (increases strength at a cost of 1 intelligence points)
- Bright (temporarily increases intelligence)
- Haste (temporarily increases dexterity)

### Casting Mechanics

Spell success is based on your Intelligence attribute. Higher Intelligence increases the chance of successful spells.

## Castle Layout

### Castle Generation

The castle is randomly generated at the start of each game, ensuring a unique experience every time.

### Room Types

- Empty rooms
- Stairs (up and down)
- Pools (can be drunk from for random effects)
- Chests (can contain gold or traps)
- Crystal Orbs (can be gazed into for information)
- Books (can be read for various effects); beware their is a chance a book will stick to your hand or make you blind
- Monster rooms
- Vendor rooms
- Warp rooms (teleport you to a random location)
- Sinkhole (drops you to the level below)

### Navigation and Mapping

Use the 'M' command to view a map of the current level. Unexplored rooms are hidden until discovered.

## Development Guide

### Project Structure

The project contains multiple implementations:
- C implementation (fully functional)
- Python implementation (incomplete)
- Original BASIC source
- Refactored BASIC source

### Building and Compiling

To build the C version:
1. Navigate to the `c/` directory
2. Run `make` to compile
3. Run `./wizardscastle` to play

For debugging, use `make debug` for memory issues and run with `./wizardscastle --debug` to make the castle easier

## Version History

Check the project's GitHub repository for the latest updates and changes.

## FAQs

### Gameplay FAQs

Q: How do I win the game?
A: Find the Orb of Zot and escape the castle with it.

Q: What does the Runestaff do?
A: The Runestaff allows you to teleport within the castle.

### Technical FAQs

Q: Can I play the original BASIC version?
A: Yes, the original BASIC source is included and can be run using Quick Basic 7.1.

Q: Is there a mobile version available?
A: Not currently, but it's a potential future development.

---

We encourage all players and developers to contribute to this wiki. If you have knowledge to share or find any inaccuracies, please feel free to edit and improve these pages. Together, we can create a comprehensive resource for all things Wizard's Castle!

For guidelines on contributing to the project itself, please refer to our [CONTRIBUTING.md](CONTRIBUTING.md) file.

Happy adventuring in the Wizard's Castle!
