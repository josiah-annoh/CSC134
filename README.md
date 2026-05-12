# CSC134 
========================================
  DUNGEON OF ECHOES — README
  M7HW1 Mini-Project
========================================

DESCRIPTION
-----------
Dungeon of Echoes is a text-based dungeon crawler written in C++.
The player chooses a character class (Warrior, Rogue, or Mage) and
descends through 5 floors of procedurally generated rooms, fighting
monsters, finding loot, visiting merchants, and ultimately facing a
boss on each floor.

HOW TO COMPILE
--------------
Using g++ (recommended):

  g++ -o dungeon dungeon_of_echoes.cpp

Then run with:

  ./dungeon          (Mac/Linux)
  dungeon.exe        (Windows)

No external libraries required — standard C++ only.

HOW TO PLAY
-----------
- Pick a name and class at the start.
- Each floor has 3–5 random rooms before a boss room.
- Room types: Monster Fight, Treasure Chest, Merchant Shop,
  Healing Spring, or Empty Room.
- In combat, choose to Attack, Heal, or Flee each turn.
- Defeat the boss on all 5 floors to win!
- Equip better weapons/armor when found to increase ATK/DEF.

FEATURES
--------
- 3 playable classes with different stats
- Scaling monster difficulty per floor
- Random room generation each run
- Loot system (weapons and armor)
- In-dungeon merchant shop
- 3 unique boss types
- Replay option after win or death

========================================
  AI ASSISTANCE CITATION
========================================

AI Used: Claude (Anthropic) — claude.ai
Model:   Claude Sonnet 4.6

Prompts given to the AI (paraphrased):
  1. "I have a C++ text-based game assignment (M7HW1 Mini-Project).
     I want to make a text adventure / dungeon crawler starting
     from scratch. Help me build it."

  2. (AI asked clarifying questions about game type and platform,
     then generated the full dungeon_of_echoes.cpp file including:
     character creation, room system, combat loop, monster/loot
     scaling, shop events, boss fights, and replay loop.)

The AI generated the complete source code and this README.
Student reviewed the code, ran it, and verified it compiles
and plays correctly before submission.

========================================