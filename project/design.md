Elemental Apocalypse — Design Document
CSC 134 | M8 Final Project | Jojo Annoh

1. Project Overview
Elemental Apocalypse is a terminal-based C++ RPG with a branching narrative and a turn-based battle system. The player chooses one of four elemental affinities, then fights through a three-boss campaign — two path-specific titans and a final Void Colossus — to save the world from an elemental apocalypse.

2. Program Structure
m5lab1_Annoh.cpp        — Entry point, getPlayerChoice(), showChoices(), main()
m5lab1_elemental.h      — All game logic: story branches, battle system, campaign flow
The header-only approach keeps all game logic in one file for easy reading. The .cpp handles only I/O utilities and main().

3. Element System
Player Elements
ElementSTRHPMPPotionsEtherStrength/WeaknessFire121208032Strong vs Air, weak vs WaterLightning1111010033Strong vs Air, weak vs WaterAir101009032Strong vs Water, weak vs LightningWater111159042Strong vs Fire/Lightning, weak vs Air
Matchup Multiplier Table
Attacker →vs Firevs Watervs Lightningvs AirFire—0.7x1.0x1.4xWater1.4x—1.4x0.7xLightning1.0x0.7x—1.4xAir0.7x1.4x0.7x—Void1.0x1.0x1.0x1.0x

4. Campaign Structure
Each element has a unique two-boss path, followed by the shared final boss.
Fire Path:      Tidal Titan (Water) → Storm Titan (Lightning) → The Void Colossus
Lightning Path: Cyclone Titan (Air) → Magma Titan (Fire)     → The Void Colossus
Air Path:       Storm Titan (Lightning) → Tidal Titan (Water) → The Void Colossus
Water Path:     Magma Titan (Fire) → Cyclone Titan (Air)      → The Void Colossus
Between each fight, the player receives:

~1/3 HP and MP restored (partial, not full — keeps tension)
+2 STR, +15 Max HP, +10 Max MP
1 bonus Potion and Ether


5. Story Branches (Pre-Battle)
Before each boss, the player chooses one of three approaches:
ChoiceEffectScoutReveals the boss's rage HP threshold before the fight startsRush InDeals a free opening strike (2x STR × matchup multiplier), no intelRetreatGains +1 Potion and +1 Ether (or boss gains +2 STR if already stocked)

6. Battle System
Player Actions (9 options)

Basic Attack — free, STR + rand(0–4), 15% crit (×1.5)
Ability 1 — moderate damage, element-scaled, mana cost
Ability 2 — heavy damage, element-scaled, higher mana cost
Ability 3 — class special: Fire=shield, Lightning=stun, Air=dodge, Water=heal
Defend — free, reduces next incoming hit by 60%
Use Potion — restores 40 HP (capped at max)
Use Ether — restores 30 MP (capped at max)
Inventory — displays item counts
Flee — costs 1 Potion; boss recovers 15% HP; returns to pre-battle

Boss Move Patterns
MoveNormal PhaseRage PhaseNotesHeavy Strike40%40%15% boss crit (×2.0); +50% if chargedQuick Jab ×230%30%Two hits; dodge only blocks firstCharge Up30%—Skips turn; next Heavy Strike ×1.5Rage Special—30%Unique ability per boss element
Rage Phase (below 40% HP)
Each boss element has a unique rage ability:

Water — Tidal Surge: heavy hit + boss self-heals 15–25 HP
Air — Cyclone Shred: 3 hits; dodge avoids all three
Lightning — Static Prison: paralysis damage + free follow-up hit
Fire — Magma Eruption: heavy hit + 10–15 burn damage
Void — randomly selects one of the four abilities above each rage turn

Status Effects
StatusSourceDurationEffectStunnedLightning Ability 31 turnBoss skips its actionDodgingAir Ability 31 turnFully avoids next attackShieldedFire Ability 31 turnIncoming damage ÷ 2DefendingPlayer action1 turnIncoming damage × 0.4 (60% off)ChargingBoss move 31 turn setupBoss Heavy Strike deals ×1.5

7. Input Validation
getPlayerChoice(int maxChoice) in the .cpp:

Uses cin >> int inside a loop
On non-integer input: clears cin error flags, flushes the line buffer, re-prompts
On out-of-range integer: prints a specific error and re-prompts
Never crashes or produces undefined behavior on bad input


8. End-of-Game Statistics
After completing the full campaign (or dying), the game reports:

Element chosen
Number of bosses defeated
Total damage dealt across all battles
Total damage taken across all battles
HP remaining (if victorious)


9. Known Limitations / Future Ideas

No save system — a full run must be completed in one session
Abilities do not scale with stat growth between fights (could add level-up ability upgrades)
The Void Colossus could eventually have a second rage phase at 20% HP
A second playable path (e.g., an unlockable Void element) would add replayability