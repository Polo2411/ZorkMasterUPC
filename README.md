# Devil May Zork
Author: Jordi Polo

## Overview
Devil May Zork is a text-based adventure game inspired by the classic Zork series and heavily influenced by the Devil May Cry universe. You control **Dante**, the legendary devil hunter and son of Sparda, as you explore a demon-infested castle, gather weapons and items, and ultimately face off against your brother **Vergil** in a climactic showdown. The game is written in C++ and follows a design similar to traditional Zork-like text adventures, using rooms, exits, items, and creatures.

## Game Setting
- **Main Hall**: The starting location – a grand entrance hall with towering pillars. You find basic starting gear here, including a **DevilSword** and **HunterBag**.
- **Devil Kitchen**: Accessible west from the Main Hall (door is already open). Inhabited by a lesser demon, and contains a key to the next locked area.
- **Long Hallway**: Located east from the Main Hall (locked initially). Home to another demon, as well as bullets, a firearm, and an **Upgrader** device.
- **Devil Bathroom**: Reached from the Long Hallway’s east door (open). Contains a special key to unlock the Knight’s Chambers.
- **Knight's Chambers**: Guarded by a **DemonKnight**, with additional bullets, potions, and an **Upgrader**.
- **Devil Guest Room**: East of the Devil Kitchen. Contains more bullets, potions, and a key leading further to the Death Hall.
- **Death Hall**: A grim location harboring multiple demons, plus a final key that unlocks the King’s Room.
- **King's Room**: The final area, where **Vergil** awaits. Defeat him to complete the game!

## How to Play
After compiling and running, you’ll be placed in the **Main Hall** with a prompt. Type text commands to interact with the world.

### Basic Commands
- **look**  
  - `look`: Shows the full room description.  
  - `look sword`: Inspects a specific item if nearby or in your inventory.
- **move [direction]**  
  - E.g. `move north` to move within the current room to the north direction.
- **exit [direction]**  
  - If there’s an open Exit in that direction, you travel to another room. E.g. `exit east`
- **take [item]** / **drop [item]**  
  - Pick up or drop items in your current direction.  
  - `take DevilSword` or `drop KeyDevilHallway`
- **open [door/bag]** / **close [door/bag]**  
  - `open HunterBag` / `close KitchenDoor`
- **save [item] in [bag]**  
  - Places an inventory item inside an open bag. E.g. `save Bullet in HunterBag`
- **take [item] from [bag]**  
  - Takes an item out of an open bag.
- **attack [enemy]**  
  - Melee attack using a sword or bare hands if no sword.
- **shoot [enemy]**  
  - Fire your gun if you have one and bullets.
- **use [item]** / **use [item] in [targetItem]**  
  - `use HealthPotion` for healing or `use ArcaneUpgrader in DevilSword` to boost sword damage.
- **transform**  
  - Activates **Devil Trigger** if you meet the conditions (three consecutive attacks/shoots). Boosts damage for two turns.
- **status**  
  - Shows current health, location, devil trigger status, and inventory details.

### Doors & Keys
Certain doors are **closed** or **locked** initially. You open them with keys that match the corresponding destination.

### Movement Within a Room
Rooms have directions: **north**, **south**, **east**, **west**, or **center**.  
- `move north` to shift your position within the same room.  
- `exit north` to go to another room if a door is there and open.

## Winning & Losing
- **Losing**: If your HP hits 0, you die. Game Over.  
- **Winning**: Slay **Vergil** in the **King’s Room**. Once his HP drops to 0, you see a special victory message and end the game.

## Guide to Finishing the Game
1. **Main Hall**: Collect **DevilSword** (south), **HunterBag** (north).
2. **Devil Kitchen** (west door is open): Defeat the **KitchenDemon**, get **KeyDevilHallway** in the south, and there’s a **HealthPotion** in the north.
3. **Open Long Hallway** (east from Main Hall): Unlock it with KeyDevilHallway. Inside are demon(s), **Gun**, **Bullet**, and an **Upgrader**.
4. **Devil Bathroom** (east from Long Hallway): Contains **KeyKnightChambers** (north) and a **VitalityPotion** (east).
5. **Knight's Chambers** (north from Long Hallway, locked): Use KeyKnightChambers. Face the **DevilKnight**; gather bullets, potions, and an **Upgrader**.
6. **Devil Guest Room** (east from Devil Kitchen): Holds more bullets, potions, and **KeyDeathHall** (south).
7. **Death Hall** (north from the Devil Guest Room, locked with KeyDeathHall): Contains more demons, items, and the **KeyKingsRoom** in the south direction.
8. **King's Room** (north from Death Hall, locked): Use KeyKingsRoom. Face **Vergil**. Attack or shoot until he’s defeated.
9. **Victory**: Once Vergil’s HP is gone, you win.

## References
- **Zork**: A foundational text adventure (Tim Anderson, Marc Blank, Dave Lebling, Bruce Daniels).
- **Devil May Cry**: Capcom’s hack-and-slash franchise featuring Dante and Vergil.

## License & Repository
GitHub repository link: [https://github.com/Polo2411/ZorkMasterUPC.git](https://github.com/Polo2411/ZorkMasterUPC.git)

This project is released under the MIT License:

MIT License

Copyright (c) [2025] [Jordi Polo Tormo]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



