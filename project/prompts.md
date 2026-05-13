prompts.md — AI Collaboration Log
CSC 134 | M8 Final Project | Jojo Annoh
This file documents how I used AI assistance throughout the project, what I tried, what worked, and what I had to iterate on or fix myself.

Session 1 — M5Lab1 Initial Build
Goal: Build the base battle system and element choice system.
Prompt that worked well:

"I'm building a C++ text RPG. I want the player to choose from 4 elements (Fire, Lightning, Air, Water) and fight a boss. Each element should have a matchup advantage/disadvantage. Give me a header file with global stats, a matchup multiplier function, and a basic battle loop."

What I got: A working skeleton with getMatchupMultiplier(), global player stats, and a while loop battle. The structure was solid but the boss only had one attack pattern (boring).
What I changed: Added boss move variety (Heavy Strike / Quick Jab / Charge Up) myself, then asked Claude to help balance the damage numbers so fights weren't over in 3 turns.

Session 2 — Story Branches
Goal: Add pre-battle narrative choices (scout, rush, retreat).
Prompt:

"Add three pre-battle choices before each boss fight. Scouting should reveal the boss's rage HP threshold. Rushing in should deal a free opening hit. Retreating should give the player items. Pass boss data through function parameters."

Result: The three choice_* functions came out cleanly. The retreat function originally called choice_pre_battle() recursively, which could loop forever. I noticed that and asked:
Follow-up prompt:

"The retreat option calls choice_pre_battle again which could infinite-loop if the player keeps retreating. Fix this so retreating goes straight to battle instead."

Fix applied: choice_mid_battle_retreat now calls start_battle directly instead of re-entering the pre-battle menu. Also added a check: if the player is already well-stocked, the boss gets stronger instead of the player getting free items.

Session 3 — Battle System Expansion
Goal: Add Defend action, Flee option, critical hits for boss, status effect cleanup.
Prompt:

"Add a Defend action (free, reduces next hit by 60%) and a Flee option (costs 1 potion, boss recovers 15% HP, returns to pre-battle). Make sure dodge, shield, and defend can stack correctly. Add boss critical hits at 2x multiplier."

What I got: The defend flag and flee logic worked. The stacking order (defending checked first, then shielded) was correct.
Issue I found: The apply_hit lambda used [&] capture which requires C++11. I verified the Codespaces environment uses g++ with C++11 support by default, so this was fine. Added a note in the source.

Session 4 — Multi-Boss Campaign
Goal: Chain three boss fights with stat growth between them.
Prompt:

"Add a campaign system: each element fights two path-specific bosses then a shared final boss (The Void Colossus). Between fights, the player gets partial HP/MP restore and a stat boost (+2 STR, +15 HP, +10 MP). The Void Colossus has no element advantage against it but uses all four rage specials randomly."

Result: campaign_next_boss() and level_up_between_fights() came out well. The BossData struct was my idea — I asked Claude to structure the boss roster as arrays of structs instead of hardcoded strings everywhere.
What I adjusted: The final boss's HP (260) felt too low in playtesting — a Fire player with advantage could shred it in a few turns. Bumped it from 220 to 260 and raised its strength from 15 to 16.

Session 5 — Input Validation
Goal: Make getPlayerChoice robust against bad input.
Prompt:

"Rewrite getPlayerChoice so it handles non-integer input (letters, symbols) without crashing, and rejects out-of-range numbers. Use cin.clear() and cin.ignore() properly."

Result: Worked on first try. The key was cin.ignore(numeric_limits<streamsize>::max(), '\n') to flush the full line — I asked Claude to explain why that was better than cin.ignore(1) (because ignore(1) only skips one character, leaving garbage in the buffer).

Session 6 — Design Doc & Final Polish
Goal: Write design.md and clean up formatting/UI.
Prompt:

"Help me write a design document for this C++ RPG covering: element stats table, matchup chart, boss roster, story branch effects, battle move probabilities, status effects, and input validation approach."

Result: Got a solid draft. I added the Known Limitations section myself because the AI didn't include any (it tends to only document what works, not what's missing).
UI cleanup: Added printDivider() and printHeader() utility functions so the terminal output has consistent separators. The AI suggested using \033[1m ANSI bold codes, but I left those out since terminal support varies and the assignment doesn't require it.

What I Learned from AI Collaboration

Be specific about parameters and architecture — vague prompts like "make a battle system" get vague results. "Add a Defend action as choice 5 that sets a defending bool, reducing the next hit by 60%" gets exactly what you need.
Always test before moving on — several times the generated code compiled fine but had logic bugs (the infinite retreat loop, the boss HP being too low). AI doesn't playtest.
Use AI to explain, not just generate — asking "why does cin.ignore need numeric_limits::max()" taught me more than just copying the fix.
The design doc helps the AI too — once I wrote the matchup table and boss roster myself in pseudocode, giving that to the AI produced much cleaner code than describing it in prose.