/*
CSC 134
M5Lab1 - Elemental Apocalypse
m5lab1_elemental.h - Story branches, battle system, all game logic
Jojo Annoh
*/

#ifndef M5LAB1_ELEMENTAL_H
#define M5LAB1_ELEMENTAL_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// ---- UTILITY PROTOTYPES (defined in .cpp) ----
extern int getPlayerChoice(int maxChoice);
extern void showChoices(string c1, string c2, string c3);

// ---- STORY BRANCH PROTOTYPES ----
void game_start();
void choose_element();
void choice_pre_battle(string boss_name, string boss_element, int boss_hp, int boss_strength);
void choice_scout(string boss_name, string boss_element, int boss_hp, int boss_strength);
void choice_rush_in(string boss_name, string boss_element, int boss_hp, int boss_strength);
void choice_mid_battle_retreat();

// ---- ELEMENT FUNCTION PROTOTYPES ----
void Fire();
void Lightning();
void Air();
void Water();

// ---- BATTLE SYSTEM PROTOTYPES ----
void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength, bool scouted);
void displayHPBar(string label, int current, int max, int barWidth);
void displayHP(int playerHP, int bossHP, string bossName, int playerMaxHP, int bossMaxHP);
void displayInventory();
void usePotion();
void useEther();
float getMatchupMultiplier(string attacker, string defender);

// ============================================================
// ---- GLOBAL VARS ----
// ============================================================
string elemental_name;
int player_strength;
int player_hp;
int player_max_hp;
int player_mana;
int player_max_mana;
int potions;
int ether;
int ability1_cost;
int ability2_cost;
int ability3_cost;
string ability1_name;
string ability2_name;
string ability3_name;

// ---- BALANCE CONSTANTS ----
const int BOSS_MIN_ATTACK = 8;
const int BOSS_MAX_ATTACK = 15;

int fire_strength      = 12;
int lightning_strength = 11;
int air_strength       = 10;
int water_strength     = 11;

int fire_hp      = 120;
int lightning_hp = 110;
int air_hp       = 100;
int water_hp     = 115;

int fire_mana      = 80;
int lightning_mana = 100;
int air_mana       = 90;
int water_mana     = 90;

// ============================================================
// ---- UTILITY FUNCTIONS ----
// ============================================================

float getMatchupMultiplier(string attacker, string defender) {
    if (attacker == "Fire") {
        if (defender == "Air")       return 1.4f;
        if (defender == "Water")     return 0.7f;
        if (defender == "Lightning") return 1.0f;
    }
    if (attacker == "Water") {
        if (defender == "Fire")      return 1.4f;
        if (defender == "Lightning") return 1.4f;
        if (defender == "Air")       return 0.7f;
    }
    if (attacker == "Lightning") {
        if (defender == "Air")       return 1.4f;
        if (defender == "Water")     return 0.7f;
        if (defender == "Fire")      return 1.0f;
    }
    if (attacker == "Air") {
        if (defender == "Lightning") return 0.7f;
        if (defender == "Fire")      return 0.7f;
        if (defender == "Water")     return 1.4f;
    }
    return 1.0f;
}

void displayHPBar(string label, int current, int max, int barWidth = 20) {
    int filled = (int)((float)current / max * barWidth);
    if (filled < 0) filled = 0;
    cout << label << " [";
    for (int i = 0; i < barWidth; i++) {
        if (i < filled) cout << "#";
        else cout << "-";
    }
    cout << "] " << current << "/" << max << endl;
}

void displayHP(int playerHP, int bossHP, string bossName, int playerMaxHP, int bossMaxHP) {
    cout << "==========================================" << endl;
    displayHPBar("YOU  ", playerHP, playerMaxHP);
    displayHPBar(bossName.substr(0, 5), bossHP, bossMaxHP);
    cout << "Mana: " << player_mana << "/" << player_max_mana;
    cout << "   Potions: " << potions << "   Ether: " << ether << endl;
    cout << "==========================================" << endl;
}

void displayInventory() {
    cout << endl << "---- INVENTORY ----" << endl;
    cout << "Potions (restore 40 HP): " << potions << endl;
    cout << "Ether   (restore 30 MP): " << ether << endl;
    cout << "-------------------" << endl;
}

void usePotion() {
    if (potions > 0) {
        int heal = 40;
        player_hp += heal;
        if (player_hp > player_max_hp) player_hp = player_max_hp;
        potions--;
        cout << "You drink a potion and restore " << heal << " HP!" << endl;
    } else {
        cout << "You have no potions left!" << endl;
    }
}

void useEther() {
    if (ether > 0) {
        int restore = 30;
        player_mana += restore;
        if (player_mana > player_max_mana) player_mana = player_max_mana;
        ether--;
        cout << "You drink an ether and restore " << restore << " MP!" << endl;
    } else {
        cout << "You have no ether left!" << endl;
    }
}

// ============================================================
// ---- STORY BRANCH: GAME START ----
// ============================================================

void game_start() {
    cout << R"(
==========================================
          ELEMENTAL APOCALYPSE
==========================================

The ancient seals have broken.
Four corrupted Elemental Titans have risen,
each one a force of pure destruction
tearing the world apart piece by piece.

You are the last Elemental Guardian.
Choose your power. Choose your fate.
)" << endl;

    choose_element();
}

// ============================================================
// ---- STORY BRANCH: CHOOSE ELEMENT ----
// ============================================================

void choose_element() {
    cout << "Choose Your Element:" << endl;
    cout << "1. Fire      - High attack, lower HP. Aggressive playstyle." << endl;
    cout << "2. Lightning - Balanced attack, high mana. Speed-based abilities." << endl;
    cout << "3. Air       - Lower attack, highest dodge. Tricky playstyle." << endl;
    cout << "4. Water     - Balanced all around. Strong sustain and healing." << endl;
    cout << endl;

    int choice = getPlayerChoice(4);

    if (choice == 1)      Fire();
    else if (choice == 2) Lightning();
    else if (choice == 3) Air();
    else                  Water();
}

// ============================================================
// ---- STORY BRANCH: PRE-BATTLE CHOICE ----
// Two new story branch functions required by Part 2.
// choice_pre_battle branches into choice_scout or choice_rush_in.
// ============================================================

void choice_pre_battle(string boss_name, string boss_element, int boss_hp, int boss_strength) {
    cout << "\nYou stand at the edge of the battlefield." << endl;
    cout << "The " << boss_name << " looms ahead, corrupted energy radiating off its body." << endl;
    cout << "You have a moment before it notices you." << endl;

    showChoices(
        "Scout it out — observe its patterns before engaging.",
        "Rush in immediately — no hesitation.",
        "Fall back and resupply before the fight."
    );

    int choice = getPlayerChoice(3);

    if (choice == 1) {
        choice_scout(boss_name, boss_element, boss_hp, boss_strength);
    } else if (choice == 2) {
        choice_rush_in(boss_name, boss_element, boss_hp, boss_strength);
    } else {
        choice_mid_battle_retreat();
        // After retreating and resupplying, still need to fight
        choice_pre_battle(boss_name, boss_element, boss_hp, boss_strength);
    }
}

// ---- NEW STORY BRANCH 1: SCOUT ----
// Scouting reveals the boss's rage phase trigger turn so the
// player knows exactly when the fight gets dangerous.
void choice_scout(string boss_name, string boss_element, int boss_hp, int boss_strength) {
    cout << "\nYou crouch behind the rubble and watch." << endl;
    cout << "The " << boss_name << " moves in patterns — predictable at first, then erratic." << endl;
    cout << "You notice: when it takes serious damage, its behavior completely changes." << endl;

    // Rage triggers when boss hits 40% HP
    int rage_hp_threshold = (int)(boss_hp * 0.4f);
    cout << ">> INTEL: The " << boss_name << " will enter RAGE MODE below " 
         << rage_hp_threshold << " HP." << endl;
    cout << ">> Its attacks will change. Be ready." << endl;
    cout << "\nYou slip into the arena. The " << boss_name << " turns toward you..." << endl;

    start_battle(boss_name, boss_element, boss_hp, boss_strength, true);
}

// ---- NEW STORY BRANCH 2: RUSH IN ----
// Rushing in gives you a bravado buff — a free bonus to your
// first attack — but you enter with no intel on the boss.
void choice_rush_in(string boss_name, string boss_element, int boss_hp, int boss_strength) {
    cout << "\nYou charge straight at it." << endl;
    cout << "No hesitation. No plan. Pure elemental fury." << endl;
    cout << "The " << boss_name << " barely has time to react before your first strike lands." << endl;
    cout << ">> BRAVADO BONUS: Your opening attack deals double damage!" << endl;

    // Apply bravado buff: give player a temporary strength spike
    // by boosting boss HP down slightly to simulate a free opening hit
    float multiplier = getMatchupMultiplier(elemental_name, boss_element);
    int bravado_hit = (int)(player_strength * 2.0f * multiplier);
    boss_hp -= bravado_hit;
    if (boss_hp < 1) boss_hp = 1; // can't one-shot the boss

    cout << "Your opening strike deals " << bravado_hit << " damage!" << endl;
    cout << "The " << boss_name << " ROARS. The battle begins." << endl;

    start_battle(boss_name, boss_element, boss_hp, boss_strength, false);
}

// ---- NEW STORY BRANCH 3: RETREAT / RESUPPLY ----
// Player falls back to resupply — gains 1 potion and 1 ether,
// but the boss also gains strength from waiting (boss_strength is
// handled in the caller, so here we just narrate and restore items).
void choice_mid_battle_retreat() {
    cout << "\nYou pull back, putting rubble between you and the titan." << endl;
    cout << "Heart pounding, you dig through your pack." << endl;
    cout << ">> You found a spare Potion and an Ether in your gear!" << endl;
    potions += 1;
    ether += 1;
    cout << "But the ground trembles. The " << endl;
    cout << "titan is growing more agitated..." << endl;
    cout << "You have to go back in." << endl;
}

// ============================================================
// ---- ELEMENT SETUP FUNCTIONS ----
// ============================================================

void Fire() {
    elemental_name = "Fire";
    player_strength = fire_strength;
    player_hp = player_max_hp = fire_hp;
    player_mana = player_max_mana = fire_mana;
    potions = 3; ether = 2;

    ability1_name = "Flame Surge";   ability1_cost = 15;
    ability2_name = "Inferno Burst"; ability2_cost = 25;
    ability3_name = "Ember Shield";  ability3_cost = 20;

    cout << endl << "Flames surge through your veins." << endl;
    cout << "The ground chars with every step as you march toward the corruption." << endl;
    cout << "Ahead, a colossal Tidal Titan rises from a flooded crater." << endl;
    cout << "Its body is a living ocean — and it has your scent." << endl;

    choice_pre_battle("Tidal Titan", "Water", 180, 13);
}

void Lightning() {
    elemental_name = "Lightning";
    player_strength = lightning_strength;
    player_hp = player_max_hp = lightning_hp;
    player_mana = player_max_mana = lightning_mana;
    potions = 3; ether = 3;

    ability1_name = "Volt Strike";  ability1_cost = 12;
    ability2_name = "Thunder Clap"; ability2_cost = 28;
    ability3_name = "Static Field"; ability3_cost = 18;

    cout << endl << "Lightning crackles across your skin." << endl;
    cout << "You move like a bolt, crossing the shattered skybridge in seconds." << endl;
    cout << "At the edge of the world, a Cyclone Titan screams into existence." << endl;
    cout << "Winds howl so loud the ground itself cracks beneath you." << endl;

    choice_pre_battle("Cyclone Titan", "Air", 165, 12);
}

void Air() {
    elemental_name = "Air";
    player_strength = air_strength;
    player_hp = player_max_hp = air_hp;
    player_mana = player_max_mana = air_mana;
    potions = 3; ether = 2;

    ability1_name = "Wind Slash";    ability1_cost = 10;
    ability2_name = "Vacuum Vortex"; ability2_cost = 22;
    ability3_name = "Gale Dodge";    ability3_cost = 15;

    cout << endl << "You dissolve into the wind." << endl;
    cout << "Invisible and weightless, you drift through the ruined canyon." << endl;
    cout << "A Storm Titan crackles to life ahead, electricity arcing from its body." << endl;
    cout << "The air around it smells like burning metal." << endl;

    choice_pre_battle("Storm Titan", "Lightning", 155, 11);
}

void Water() {
    elemental_name = "Water";
    player_strength = water_strength;
    player_hp = player_max_hp = water_hp;
    player_mana = player_max_mana = water_mana;
    potions = 4; ether = 2;

    ability1_name = "Tidal Wave";      ability1_cost = 15;
    ability2_name = "Tsunami Crush";   ability2_cost = 26;
    ability3_name = "Healing Current"; ability3_cost = 20;

    cout << endl << "A wave crashes over you and recedes." << endl;
    cout << "Tides pulse through your body as you surge through the burning forest." << endl;
    cout << "At the end of the trail, a Magma Titan erupts from the earth." << endl;
    cout << "Lava drips from its fists. The trees around it are already ash." << endl;

    choice_pre_battle("Magma Titan", "Fire", 175, 13);
}

// ============================================================
// ---- BATTLE SYSTEM ----
// ============================================================
//
// WHAT'S NEW vs M3 Bonus:
//  1. Boss Rage Phase  - below 40% HP, boss enters rage mode with a
//                        brand new special attack unique to their element.
//  2. Boss Move Variety- boss randomly picks from 3 attack patterns each
//                        turn instead of always doing the same hit:
//                          a) Heavy Strike  - slow but hits hard
//                          b) Quick Jab x2  - two small hits in one turn
//                          c) Charge Up     - boss skips attack, then next
//                                             turn hits for massive damage
//  3. Critical Hits    - both player and boss have a 15% crit chance.
//                        Player crits deal 1.5x damage. Boss crits deal 2x.
//  4. Scouted flag     - if you scouted, rage threshold was revealed early.
//
// ============================================================

void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength, bool scouted) {

    int boss_max_hp = boss_hp;
    float multiplier = getMatchupMultiplier(elemental_name, boss_element);

    // Combat state flags
    bool stunned      = false;
    bool dodging      = false;
    bool shielded     = false;
    bool boss_charging = false;   // boss skipped last turn to charge
    bool in_rage      = false;    // boss rage phase active
    int turn = 1;

    // Rage triggers at 40% HP
    int rage_threshold = (int)(boss_max_hp * 0.4f);

    cout << "\n---- MATCHUP ----" << endl;
    if (multiplier > 1.0f)
        cout << ">> ADVANTAGE: Your " << elemental_name << " is strong vs " << boss_element << "!" << endl;
    else if (multiplier < 1.0f)
        cout << ">> DISADVANTAGE: Your " << elemental_name << " is weak vs " << boss_element << "." << endl;
    else
        cout << ">> NEUTRAL matchup." << endl;
    cout << endl;

    // ---- BATTLE LOOP ----
    while (player_hp > 0 && boss_hp > 0) {

        // Check if boss should enter rage phase
        if (!in_rage && boss_hp <= rage_threshold) {
            in_rage = true;
            cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            cout << "!! " << boss_name << " ENTERS RAGE MODE !!" << endl;
            cout << "!! Its attacks are changing. Brace yourself." << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        }

        cout << endl << "-- Turn " << turn << " --";
        if (in_rage) cout << " [RAGE]";
        cout << endl;

        displayHP(player_hp, boss_hp, boss_name, player_max_hp, boss_max_hp);

        if (boss_charging) {
            cout << ">> WARNING: " << boss_name << " is CHARGED UP — its next hit will be massive!" << endl;
        }

        cout << endl << "What do you do?" << endl;
        cout << "1. Basic Attack          (deals ~" << player_strength << " dmg, free)" << endl;
        cout << "2. " << ability1_name << "   (deals ~" << (int)(player_strength * 1.6f * multiplier) 
             << " dmg, " << ability1_cost << " MP)" << endl;
        cout << "3. " << ability2_name << "   (deals ~" << (int)(player_strength * 2.5f * multiplier) 
             << " dmg, " << ability2_cost << " MP)" << endl;
        cout << "4. " << ability3_name << "   (" << ability3_cost << " MP - special)" << endl;
        cout << "5. Use Potion            (restore 40 HP)" << endl;
        cout << "6. Use Ether             (restore 30 MP)" << endl;
        cout << "7. Inventory" << endl;

        int choice = getPlayerChoice(7);

        int damage_dealt = 0;
        dodging  = false;
        shielded = false;

        // ---- PLAYER ACTION ----
        if (choice == 1) {
            // Basic attack — always free, small random variance
            damage_dealt = player_strength + (rand() % 5);

            // 15% crit chance
            if (rand() % 100 < 15) {
                damage_dealt = (int)(damage_dealt * 1.5f);
                cout << "** CRITICAL HIT! **" << endl;
            }
            cout << "You strike for " << damage_dealt << " damage!" << endl;

        } else if (choice == 2) {
            if (player_mana >= ability1_cost) {
                player_mana -= ability1_cost;
                damage_dealt = (int)(player_strength * 1.6f * multiplier) + (rand() % 6);

                if (rand() % 100 < 15) {
                    damage_dealt = (int)(damage_dealt * 1.5f);
                    cout << "** CRITICAL HIT! **" << endl;
                }
                cout << "You unleash " << ability1_name << " for " << damage_dealt << " damage!" << endl;
                if (multiplier > 1.0f) cout << "It's super effective!" << endl;
                if (multiplier < 1.0f) cout << "The boss resists your element..." << endl;
            } else {
                cout << "Not enough mana! (" << ability1_cost << " MP needed)" << endl;
            }

        } else if (choice == 3) {
            if (player_mana >= ability2_cost) {
                player_mana -= ability2_cost;
                damage_dealt = (int)(player_strength * 2.5f * multiplier) + (rand() % 8);

                if (rand() % 100 < 15) {
                    damage_dealt = (int)(damage_dealt * 1.5f);
                    cout << "** CRITICAL HIT! **" << endl;
                }
                cout << "You UNLEASH " << ability2_name << " for " << damage_dealt << " damage!" << endl;
                if (multiplier > 1.0f) cout << "CRITICAL ADVANTAGE — the boss staggers!" << endl;
                if (multiplier < 1.0f) cout << "The boss absorbs most of it..." << endl;
            } else {
                cout << "Not enough mana! (" << ability2_cost << " MP needed)" << endl;
            }

        } else if (choice == 4) {
            if (player_mana >= ability3_cost) {
                player_mana -= ability3_cost;

                if (elemental_name == "Fire") {
                    shielded = true;
                    cout << "Ember Shield activated! Next hit reduced by half!" << endl;
                } else if (elemental_name == "Lightning") {
                    stunned = true;
                    cout << "Static Field crackles! " << boss_name << " is STUNNED!" << endl;
                } else if (elemental_name == "Air") {
                    dodging = true;
                    cout << "You dissolve into wind — you will dodge the next attack!" << endl;
                } else if (elemental_name == "Water") {
                    int heal = 30 + (rand() % 15);
                    player_hp += heal;
                    if (player_hp > player_max_hp) player_hp = player_max_hp;
                    cout << "Healing Current flows — restored " << heal << " HP!" << endl;
                }
            } else {
                cout << "Not enough mana! (" << ability3_cost << " MP needed)" << endl;
            }

        } else if (choice == 5) {
            usePotion();
        } else if (choice == 6) {
            useEther();
        } else if (choice == 7) {
            displayInventory();
        }

        // Apply player damage to boss
        boss_hp -= damage_dealt;
        if (boss_hp < 0) boss_hp = 0;

        // ---- BOSS TURN ----
        if (boss_hp > 0) {
            cout << endl;

            if (stunned) {
                cout << boss_name << " sparks and stutters — STUNNED! Skipping their turn!" << endl;
                stunned = false;

            } else {
                // ---- BOSS MOVE SELECTION ----
                // Boss randomly picks from 3 attack patterns.
                // In rage phase, pattern 3 becomes the rage special instead.
                int boss_move;
                if (in_rage) {
                    // Rage: 40% heavy, 30% double jab, 30% rage special
                    int roll = rand() % 10;
                    if (roll < 4)      boss_move = 1; // heavy strike
                    else if (roll < 7) boss_move = 2; // double jab
                    else               boss_move = 4; // rage special
                } else {
                    // Normal: 40% heavy, 30% double jab, 30% charge up
                    int roll = rand() % 10;
                    if (roll < 4)      boss_move = 1;
                    else if (roll < 7) boss_move = 2;
                    else               boss_move = 3; // charge up
                }

                if (boss_move == 1) {
                    // ---- HEAVY STRIKE ----
                    // Hits harder than normal, 15% crit chance for boss too
                    int boss_dmg = (int)(boss_strength * 1.3f) + (rand() % (BOSS_MAX_ATTACK - BOSS_MIN_ATTACK + 1));

                    // Boss crit is nastier — 2x instead of 1.5x
                    bool boss_crit = (rand() % 100 < 15);
                    if (boss_crit) boss_dmg = (int)(boss_dmg * 2.0f);

                    if (dodging) {
                        cout << "You vanish into the wind — Heavy Strike misses completely!" << endl;
                    } else if (shielded) {
                        boss_dmg /= 2;
                        player_hp -= boss_dmg;
                        cout << boss_name << " winds up a HEAVY STRIKE — shield absorbs half! ";
                        cout << boss_dmg << " damage taken." << endl;
                    } else {
                        player_hp -= boss_dmg;
                        cout << boss_name << " winds up and slams you with a HEAVY STRIKE for ";
                        if (boss_crit) cout << "** " << boss_dmg << " ** BOSS CRITICAL!" << endl;
                        else cout << boss_dmg << " damage!" << endl;
                    }

                    // If boss was charged, the heavy strike fires at full power and clears the flag
                    if (boss_charging) {
                        boss_charging = false;
                    }

                } else if (boss_move == 2) {
                    // ---- QUICK JAB x2 ----
                    // Two smaller hits in one turn — dodge only blocks the first
                    int jab1 = (int)(boss_strength * 0.6f) + (rand() % 5);
                    int jab2 = (int)(boss_strength * 0.6f) + (rand() % 5);
                    cout << boss_name << " strikes TWICE in rapid succession!" << endl;

                    if (dodging) {
                        cout << "  Jab 1: You dodge the first hit!" << endl;
                        // Second hit still lands on a dodge
                        if (shielded) jab2 /= 2;
                        player_hp -= jab2;
                        cout << "  Jab 2: You couldn't dodge again — " << jab2 << " damage!" << endl;
                    } else {
                        if (shielded) { jab1 /= 2; jab2 /= 2; }
                        player_hp -= jab1;
                        player_hp -= jab2;
                        cout << "  Jab 1: " << jab1 << " damage!" << endl;
                        cout << "  Jab 2: " << jab2 << " damage!" << endl;
                        cout << "  Total: " << (jab1 + jab2) << " damage!" << endl;
                    }

                } else if (boss_move == 3) {
                    // ---- CHARGE UP (normal phase only) ----
                    // Boss skips its attack this turn but deals double next turn.
                    // The flag is set; damage fires next time boss_move == 1.
                    if (!boss_charging) {
                        boss_charging = true;
                        cout << boss_name << " pulls back and begins to CHARGE UP energy..." << endl;
                        cout << ">> Its next attack will deal double damage!" << endl;
                    } else {
                        // Boss was already charged — release it as a massive hit
                        int charged_dmg = boss_strength * 2 + (rand() % 10);
                        boss_charging = false;
                        if (dodging) {
                            cout << boss_name << " RELEASES a charged attack — but you dodge it!" << endl;
                        } else {
                            player_hp -= charged_dmg;
                            cout << boss_name << " RELEASES the charged energy for " << charged_dmg << " DAMAGE!" << endl;
                        }
                    }

                } else if (boss_move == 4) {
                    // ---- RAGE SPECIAL (rage phase only) ----
                    // Each boss element has a unique rage ability.
                    cout << "!! " << boss_name << " unleashes its RAGE ABILITY !!" << endl;
                    int rage_dmg = 0;

                    if (boss_element == "Water") {
                        // Tidal Surge — massive AOE wave, also heals boss slightly
                        rage_dmg = boss_strength * 2 + (rand() % 12);
                        int boss_heal = 15 + (rand() % 10);
                        boss_hp += boss_heal;
                        if (boss_hp > boss_max_hp) boss_hp = boss_max_hp;
                        cout << "TIDAL SURGE: A wall of water crashes over you!" << endl;
                        cout << "The titan absorbs moisture and heals " << boss_heal << " HP!" << endl;

                    } else if (boss_element == "Air") {
                        // Cyclone Shred — hits 3 times for moderate damage
                        int hit1 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        int hit2 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        int hit3 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        cout << "CYCLONE SHRED: Three cutting wind blades tear through you!" << endl;
                        if (!dodging) {
                            player_hp -= (hit1 + hit2 + hit3);
                            cout << "  Blade 1: " << hit1 << " | Blade 2: " << hit2 << " | Blade 3: " << hit3 << endl;
                            cout << "  Total: " << (hit1 + hit2 + hit3) << " damage!" << endl;
                        } else {
                            cout << "You dodge through the gaps in the blades!" << endl;
                        }

                    } else if (boss_element == "Lightning") {
                        // Static Prison — boss stuns YOU for one turn (you can't choose your action)
                        rage_dmg = boss_strength + (rand() % 8);
                        cout << "STATIC PRISON: Electricity locks your body in place!" << endl;
                        if (!dodging) {
                            player_hp -= rage_dmg;
                            cout << "You are PARALYZED for " << rage_dmg << " damage!" << endl;
                            cout << ">> You lose your NEXT turn!" << endl;
                            // Simulate losing next turn by taking a free boss hit
                            int free_hit = boss_strength + (rand() % 5);
                            player_hp -= free_hit;
                            cout << boss_name << " strikes the stunned you for another " << free_hit << "!" << endl;
                        } else {
                            cout << "Your momentum carries you out of the electrical field!" << endl;
                        }

                    } else if (boss_element == "Fire") {
                        // Magma Eruption — sets the arena on fire, dealing burn damage
                        rage_dmg = boss_strength * 2 + (rand() % 15);
                        cout << "MAGMA ERUPTION: The ground beneath you bursts with lava!" << endl;
                        if (!dodging) {
                            player_hp -= rage_dmg;
                            cout << "Lava geysers erupt around you for " << rage_dmg << " damage!" << endl;
                            // Burn effect: additional 10 damage next two turns (simulated as extra hit now)
                            int burn = 10 + (rand() % 5);
                            player_hp -= burn;
                            cout << "You're BURNING! " << burn << " additional burn damage!" << endl;
                        } else {
                            cout << "You launch off the ground before the lava hits!" << endl;
                        }
                    }

                    // Apply rage_dmg only if not a multi-hit case handled above
                    if (boss_element == "Water") {
                        if (dodging) {
                            cout << "You ride the wave — Tidal Surge misses!" << endl;
                        } else {
                            player_hp -= rage_dmg;
                            cout << "It hits for " << rage_dmg << " damage!" << endl;
                        }
                    }
                }
            }
        }

        if (player_hp < 0) player_hp = 0;
        turn++;
    }

    // ---- OUTCOME ----
    cout << endl;
    if (player_hp <= 0) {
        cout << "===========================================" << endl;
        cout << "You have fallen..." << endl;
        cout << boss_name << " lets out a horrific roar as darkness consumes the land." << endl;
        cout << "The world was not saved. The apocalypse is complete." << endl;
        cout << "===========================================" << endl;
    } else {
        cout << "===========================================" << endl;
        cout << boss_name << " collapses with a world-shaking crash!" << endl;
        cout << "The corruption shatters. Light breaks through the clouds." << endl;
        cout << "You stand victorious — your " << elemental_name << " power surging through you." << endl;
        cout << "The world breathes again. You saved it." << endl;
        cout << endl << "          *** YOU WIN ***" << endl;
        cout << "===========================================" << endl;
    }
}

#endif