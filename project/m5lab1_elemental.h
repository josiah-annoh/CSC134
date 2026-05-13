/*
CSC 134
M8 Final Project - Elemental Apocalypse (Final Version)
m5lab1_elemental.h - Story branches, battle system, full campaign logic
Jojo Annoh

CHANGELOG from M5Lab1:
  - Multi-boss campaign: each element now fights TWO bosses before a final encounter
  - New Defend action: reduces incoming damage by 60% for one turn (free, no MP)
  - Flee option: spend 1 potion to escape a losing fight and retry (costs resources)
  - Stat growth: player gains +2 STR and +15 HP/MP between boss fights
  - Full input validation in getPlayerChoice (rejects letters, out-of-range)
  - Improved terminal UI: styled headers, consistent spacing, color-coded matchup
  - design.md and prompts.md included in project deliverables
*/

#ifndef M5LAB1_ELEMENTAL_H
#define M5LAB1_ELEMENTAL_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

// ---- UTILITY PROTOTYPES (defined in .cpp) ----
extern int  getPlayerChoice(int maxChoice);
extern void showChoices(string c1, string c2, string c3);

// ---- STORY BRANCH PROTOTYPES ----
void game_start();
void choose_element();
void choice_pre_battle(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num);
void choice_scout(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num);
void choice_rush_in(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num);
void choice_mid_battle_retreat(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num);

// ---- ELEMENT FUNCTION PROTOTYPES ----
void Fire();
void Lightning();
void Air();
void Water();

// ---- CAMPAIGN PROGRESSION ----
void level_up_between_fights(int boss_num);
void campaign_next_boss(int boss_num);

// ---- BATTLE SYSTEM PROTOTYPES ----
void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength,
                  bool scouted, int boss_num);
void displayHPBar(string label, int current, int max, int barWidth);
void displayHP(int playerHP, int bossHP, string bossName, int playerMaxHP, int bossMaxHP);
void displayInventory();
void usePotion();
void useEther();
float getMatchupMultiplier(string attacker, string defender);
void printDivider(char c = '=', int width = 46);
void printHeader(string title);

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

// Track total damage dealt for end-of-game stats
int total_damage_dealt = 0;
int total_damage_taken = 0;
int battles_won       = 0;

// ---- BALANCE CONSTANTS ----
const int BOSS_MIN_ATTACK = 8;
const int BOSS_MAX_ATTACK = 15;

// Per-element base stats
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
// ---- BOSS ROSTER ----
// Each element faces 2 regular bosses then the Final Titan.
// Layout: { name, element, base_hp, base_strength }
// Boss difficulty scales: boss 1 = base, boss 2 = +20%, final = +40%
// ============================================================
// Fire path:   Tidal Titan (Water) -> Storm Titan (Lightning) -> The Void Colossus (all elements)
// Lightning:   Cyclone Titan (Air) -> Magma Titan (Fire)      -> The Void Colossus
// Air:         Storm Titan (Lightning) -> Tidal Titan (Water)  -> The Void Colossus
// Water:       Magma Titan (Fire) -> Cyclone Titan (Air)       -> The Void Colossus

struct BossData {
    string name;
    string element;
    int    hp;
    int    strength;
};

BossData firePath[2] = {
    {"Tidal Titan",   "Water",     180, 13},
    {"Storm Titan",   "Lightning", 200, 14}
};
BossData lightningPath[2] = {
    {"Cyclone Titan", "Air",       165, 12},
    {"Magma Titan",   "Fire",      185, 14}
};
BossData airPath[2] = {
    {"Storm Titan",   "Lightning", 155, 11},
    {"Tidal Titan",   "Water",     175, 13}
};
BossData waterPath[2] = {
    {"Magma Titan",   "Fire",      175, 13},
    {"Cyclone Titan", "Air",       190, 13}
};

BossData finalBoss = {"The Void Colossus", "Void", 260, 16};

// ============================================================
// ---- UI HELPERS ----
// ============================================================

void printDivider(char c, int width) {
    for (int i = 0; i < width; i++) cout << c;
    cout << endl;
}

void printHeader(string title) {
    printDivider('=');
    int pad = (46 - (int)title.size()) / 2;
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title << endl;
    printDivider('=');
}

// ============================================================
// ---- UTILITY FUNCTIONS ----
// ============================================================

float getMatchupMultiplier(string attacker, string defender) {
    // Void is neutral against everything (final boss)
    if (defender == "Void" || attacker == "Void") return 1.0f;

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
    int filled = (current > 0) ? (int)((float)current / max * barWidth) : 0;
    if (filled > barWidth) filled = barWidth;
    // Pad label to 6 chars for alignment
    while ((int)label.size() < 6) label += " ";
    cout << label << " [";
    for (int i = 0; i < barWidth; i++) cout << (i < filled ? '#' : '-');
    cout << "] " << current << "/" << max << endl;
}

void displayHP(int playerHP, int bossHP, string bossName, int playerMaxHP, int bossMaxHP) {
    printDivider('-');
    displayHPBar("YOU", playerHP, playerMaxHP);
    displayHPBar(bossName.substr(0, 6), bossHP, bossMaxHP);
    cout << "Mana: " << player_mana << "/" << player_max_mana;
    cout << "   Potions: " << potions << "   Ether: " << ether << endl;
    printDivider('-');
}

void displayInventory() {
    cout << endl;
    printDivider('-', 30);
    cout << "        INVENTORY" << endl;
    printDivider('-', 30);
    cout << "Potions (restore 40 HP) : " << potions << endl;
    cout << "Ether   (restore 30 MP) : " << ether   << endl;
    printDivider('-', 30);
}

void usePotion() {
    if (potions > 0) {
        int heal = 40;
        player_hp = min(player_hp + heal, player_max_hp);
        potions--;
        cout << "You drink a potion and restore " << heal << " HP!  [" << player_hp << "/" << player_max_hp << "]" << endl;
    } else {
        cout << "!! You have no potions left!" << endl;
    }
}

void useEther() {
    if (ether > 0) {
        int restore = 30;
        player_mana = min(player_mana + restore, player_max_mana);
        ether--;
        cout << "You drink an ether and restore " << restore << " MP!  [" << player_mana << "/" << player_max_mana << "]" << endl;
    } else {
        cout << "!! You have no ether left!" << endl;
    }
}

// ============================================================
// ---- CAMPAIGN PROGRESSION ----
// ============================================================

// Called between boss fights: restore some HP/MP and boost stats
void level_up_between_fights(int /*boss_num*/) {
    printHeader("BETWEEN BATTLES");
    cout << endl;

    // Partial restore (not full heal — keeps tension)
    int hp_restore  = player_max_hp / 3;
    int mp_restore  = player_max_mana / 3;
    player_hp   = min(player_hp   + hp_restore, player_max_hp);
    player_mana = min(player_mana + mp_restore,  player_max_mana);

    // Stat growth
    player_strength  += 2;
    player_max_hp    += 15;
    player_max_mana  += 10;
    potions          += 1;
    ether            += 1;

    cout << "The corruption retreats — but only for a moment." << endl;
    cout << "You feel your power growing as the first seal shatters." << endl;
    cout << endl;
    cout << "  >> HP restored: +" << hp_restore << "  (now " << player_hp << "/" << player_max_hp << ")" << endl;
    cout << "  >> MP restored: +" << mp_restore  << "  (now " << player_mana << "/" << player_max_mana << ")" << endl;
    cout << "  >> STR up +2   (now " << player_strength << ")" << endl;
    cout << "  >> Max HP +15, Max MP +10" << endl;
    cout << "  >> Found 1 Potion and 1 Ether in the ruins." << endl;
    cout << endl;
    cout << "The next titan is already moving. You press forward." << endl;
    cout << endl;
}

// Advances to the next boss in the campaign
void campaign_next_boss(int boss_num) {
    BossData* path = nullptr;
    if      (elemental_name == "Fire")      path = firePath;
    else if (elemental_name == "Lightning") path = lightningPath;
    else if (elemental_name == "Air")       path = airPath;
    else                                    path = waterPath;

    if (boss_num == 1) {
        // First boss down — fight boss 2
        level_up_between_fights(1);
        choice_pre_battle(path[1].name, path[1].element, path[1].hp, path[1].strength, 2);
    } else if (boss_num == 2) {
        // Second boss down — fight the final titan
        level_up_between_fights(2);

        printHeader("THE FINAL SEAL");
        cout << endl;
        cout << "Two titans fall. The sky tears open." << endl;
        cout << "From the rift descends The Void Colossus —" << endl;
        cout << "a being of pure elemental chaos, every power twisted into one." << endl;
        cout << "This is the source of the apocalypse. This ends here." << endl;
        cout << endl;

        choice_pre_battle(finalBoss.name, finalBoss.element, finalBoss.hp, finalBoss.strength, 3);
    } else {
        // Boss 3 (final) was defeated — show victory stats
        printHeader("ELEMENTAL APOCALYPSE COMPLETE");
        cout << endl;
        cout << "The Void Colossus shatters into a million fragments of light." << endl;
        cout << "Every crack in the earth seals. Every fire dies." << endl;
        cout << "The world exhales for the first time in an age." << endl;
        cout << endl;
        cout << "You are the last Guardian. You are enough." << endl;
        cout << endl;
        printDivider('*');
        cout << "           FINAL STATS" << endl;
        printDivider('*');
        cout << "  Element       : " << elemental_name << endl;
        cout << "  Bosses Slain  : " << battles_won << endl;
        cout << "  Damage Dealt  : " << total_damage_dealt << endl;
        cout << "  Damage Taken  : " << total_damage_taken << endl;
        cout << "  HP Remaining  : " << player_hp << "/" << player_max_hp << endl;
        printDivider('*');
        cout << endl;
    }
}

// ============================================================
// ---- STORY BRANCH: GAME START ----
// ============================================================

void game_start() {
    cout << R"(
==============================================
          ELEMENTAL APOCALYPSE
              FINAL VERSION
==============================================

The ancient seals have broken.
Four corrupted Elemental Titans have risen,
each one a force of pure destruction
tearing the world apart piece by piece.

But behind them all — something older waits.
The Void Colossus. The source of everything.

You are the last Elemental Guardian.
Defeat the titans. Break the source.
Choose your power. Choose your fate.
)" << endl;

    choose_element();
}

// ============================================================
// ---- STORY BRANCH: CHOOSE ELEMENT ----
// ============================================================

void choose_element() {
    printHeader("CHOOSE YOUR ELEMENT");
    cout << endl;
    cout << "1. Fire       - High attack, lower HP. Aggressive playstyle." << endl;
    cout << "2. Lightning  - Balanced attack, high mana. Speed abilities." << endl;
    cout << "3. Air        - Lower attack, highest mobility. Tricky style." << endl;
    cout << "4. Water      - Balanced all-around. Strong sustain & healing." << endl;
    cout << endl;

    int choice = getPlayerChoice(4);

    if      (choice == 1) Fire();
    else if (choice == 2) Lightning();
    else if (choice == 3) Air();
    else                  Water();
}

// ============================================================
// ---- STORY BRANCH: PRE-BATTLE CHOICES ----
// ============================================================

void choice_pre_battle(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num) {
    cout << endl;
    printDivider('-');
    if (boss_num == 3) {
        cout << "The Void Colossus towers above you." << endl;
        cout << "All four elements swirl around its form — unstable, corrupted, hungry." << endl;
    } else {
        cout << "You stand at the edge of the battlefield." << endl;
        cout << "The " << boss_name << " looms ahead, corrupted energy radiating off it." << endl;
        cout << "You have a moment before it notices you." << endl;
    }
    cout << endl;

    showChoices(
        "Scout it out -- observe its patterns before engaging.",
        "Rush in immediately -- no hesitation.",
        "Fall back and resupply before the fight."
    );

    int choice = getPlayerChoice(3);

    if (choice == 1) {
        choice_scout(boss_name, boss_element, boss_hp, boss_strength, boss_num);
    } else if (choice == 2) {
        choice_rush_in(boss_name, boss_element, boss_hp, boss_strength, boss_num);
    } else {
        choice_mid_battle_retreat(boss_name, boss_element, boss_hp, boss_strength, boss_num);
    }
}

void choice_scout(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num) {
    cout << endl;
    cout << "You crouch behind the rubble and watch." << endl;
    cout << "The " << boss_name << " moves in patterns — predictable at first, then erratic." << endl;
    cout << "When it takes serious damage, everything changes." << endl;

    int rage_hp_threshold = (int)(boss_hp * 0.4f);
    cout << endl;
    cout << ">> INTEL: The " << boss_name << " enters RAGE MODE below "
         << rage_hp_threshold << " HP." << endl;
    cout << ">> Its attacks will shift. You'll be ready." << endl;
    cout << endl;
    cout << "You slip into the arena. The " << boss_name << " turns toward you..." << endl;

    start_battle(boss_name, boss_element, boss_hp, boss_strength, true, boss_num);
}

void choice_rush_in(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num) {
    cout << endl;
    cout << "You charge straight at it." << endl;
    cout << "No hesitation. No plan. Pure elemental fury." << endl;
    cout << "The " << boss_name << " barely has time to react before your first strike lands." << endl;

    float multiplier = getMatchupMultiplier(elemental_name, boss_element);
    int bravado_hit  = (int)(player_strength * 2.0f * multiplier);
    boss_hp -= bravado_hit;
    if (boss_hp < 1) boss_hp = 1;

    cout << ">> BRAVADO BONUS: Opening strike deals " << bravado_hit << " damage!" << endl;
    total_damage_dealt += bravado_hit;
    cout << "The " << boss_name << " ROARS. The battle begins." << endl;

    start_battle(boss_name, boss_element, boss_hp, boss_strength, false, boss_num);
}

void choice_mid_battle_retreat(string boss_name, string boss_element, int boss_hp, int boss_strength, int boss_num) {
    cout << endl;
    cout << "You pull back, putting rubble between you and the titan." << endl;
    cout << "Heart pounding, you dig through your pack." << endl;

    if (potions >= 3 && ether >= 2) {
        // Already well-stocked — the titan gets stronger for waiting
        boss_strength += 2;
        cout << "You're already well-supplied — but hesitation has a cost." << endl;
        cout << ">> The " << boss_name << " grows agitated. Its STR increases by 2!" << endl;
    } else {
        potions += 1;
        ether   += 1;
        cout << ">> Found a spare Potion and Ether in your gear!" << endl;
    }

    cout << "The ground trembles. The titan is coming to you now." << endl;
    cout << "You charge back in." << endl;

    // Go straight to battle after retreating (no infinite loop)
    start_battle(boss_name, boss_element, boss_hp, boss_strength, false, boss_num);
}

// ============================================================
// ---- ELEMENT SETUP FUNCTIONS ----
// ============================================================

void Fire() {
    elemental_name  = "Fire";
    player_strength = fire_strength;
    player_hp = player_max_hp = fire_hp;
    player_mana = player_max_mana = fire_mana;
    potions = 3; ether = 2;

    ability1_name = "Flame Surge";    ability1_cost = 15;
    ability2_name = "Inferno Burst";  ability2_cost = 25;
    ability3_name = "Ember Shield";   ability3_cost = 20;

    cout << endl;
    cout << "Flames surge through your veins." << endl;
    cout << "The ground chars with every step as you march toward the corruption." << endl;
    cout << "Ahead, a colossal Tidal Titan rises from a flooded crater." << endl;
    cout << "Its body is a living ocean — and it has your scent." << endl;

    choice_pre_battle(firePath[0].name, firePath[0].element,
                      firePath[0].hp,   firePath[0].strength, 1);
}

void Lightning() {
    elemental_name  = "Lightning";
    player_strength = lightning_strength;
    player_hp = player_max_hp = lightning_hp;
    player_mana = player_max_mana = lightning_mana;
    potions = 3; ether = 3;

    ability1_name = "Volt Strike";   ability1_cost = 12;
    ability2_name = "Thunder Clap";  ability2_cost = 28;
    ability3_name = "Static Field";  ability3_cost = 18;

    cout << endl;
    cout << "Lightning crackles across your skin." << endl;
    cout << "You move like a bolt, crossing the shattered skybridge in seconds." << endl;
    cout << "At the edge of the world, a Cyclone Titan screams into existence." << endl;
    cout << "Winds howl so loud the ground itself cracks beneath you." << endl;

    choice_pre_battle(lightningPath[0].name, lightningPath[0].element,
                      lightningPath[0].hp,   lightningPath[0].strength, 1);
}

void Air() {
    elemental_name  = "Air";
    player_strength = air_strength;
    player_hp = player_max_hp = air_hp;
    player_mana = player_max_mana = air_mana;
    potions = 3; ether = 2;

    ability1_name = "Wind Slash";     ability1_cost = 10;
    ability2_name = "Vacuum Vortex";  ability2_cost = 22;
    ability3_name = "Gale Dodge";     ability3_cost = 15;

    cout << endl;
    cout << "You dissolve into the wind." << endl;
    cout << "Invisible and weightless, you drift through the ruined canyon." << endl;
    cout << "A Storm Titan crackles to life ahead, electricity arcing from its body." << endl;
    cout << "The air around it smells like burning metal." << endl;

    choice_pre_battle(airPath[0].name, airPath[0].element,
                      airPath[0].hp,   airPath[0].strength, 1);
}

void Water() {
    elemental_name  = "Water";
    player_strength = water_strength;
    player_hp = player_max_hp = water_hp;
    player_mana = player_max_mana = water_mana;
    potions = 4; ether = 2;

    ability1_name = "Tidal Wave";       ability1_cost = 15;
    ability2_name = "Tsunami Crush";    ability2_cost = 26;
    ability3_name = "Healing Current";  ability3_cost = 20;

    cout << endl;
    cout << "A wave crashes over you and recedes." << endl;
    cout << "Tides pulse through your body as you surge through the burning forest." << endl;
    cout << "At the end of the trail, a Magma Titan erupts from the earth." << endl;
    cout << "Lava drips from its fists. The trees around it are already ash." << endl;

    choice_pre_battle(waterPath[0].name, waterPath[0].element,
                      waterPath[0].hp,   waterPath[0].strength, 1);
}

// ============================================================
// ---- BATTLE SYSTEM ----
// ============================================================
//
//  NEW vs M5Lab1:
//  1. Defend action   - free, reduces next hit by 60%
//  2. Flee option     - costs 1 potion, exits battle and retries
//                       (if no potions, can't flee — must fight)
//  3. Campaign flow   - boss_num parameter drives progression
//  4. Final boss      - Void Colossus has all four rage specials,
//                       randomly selecting one each rage turn
//  5. End stats       - damage dealt/taken tracked across campaign
//  6. Input guardrail - getPlayerChoice fully validates input
//
// ============================================================

void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength,
                  bool scouted, int boss_num) {

    int boss_max_hp = boss_hp;
    float multiplier = getMatchupMultiplier(elemental_name, boss_element);

    bool stunned       = false;
    bool dodging       = false;
    bool shielded      = false;
    bool defending     = false;    // NEW: Defend action flag
    bool boss_charging = false;
    bool in_rage       = false;
    int turn = 1;

    int rage_threshold = (int)(boss_max_hp * 0.4f);

    // ---- MATCHUP DISPLAY ----
    cout << endl;
    printDivider('-', 30);
    cout << "   BOSS " << boss_num << " OF 3: " << boss_name << endl;
    printDivider('-', 30);
    if (boss_element == "Void") {
        cout << ">> FINAL BATTLE — all elements swirl within it." << endl;
    } else if (multiplier > 1.0f) {
        cout << ">> ADVANTAGE: " << elemental_name << " is strong vs " << boss_element << "!" << endl;
    } else if (multiplier < 1.0f) {
        cout << ">> DISADVANTAGE: " << elemental_name << " is weak vs " << boss_element << "." << endl;
    } else {
        cout << ">> NEUTRAL matchup." << endl;
    }
    if (scouted) {
        cout << ">> You know the rage trigger: below " << rage_threshold << " HP." << endl;
    }
    cout << endl;

    // ---- BATTLE LOOP ----
    while (player_hp > 0 && boss_hp > 0) {

        // Rage phase trigger
        if (!in_rage && boss_hp <= rage_threshold) {
            in_rage = true;
            cout << endl;
            printDivider('!', 46);
            cout << "!! " << boss_name << " ENTERS RAGE MODE !!" << endl;
            cout << "!! Attacks are changing. Brace yourself." << endl;
            printDivider('!', 46);
        }

        cout << endl << "-- Turn " << turn << " --";
        if (in_rage) cout << " [RAGE]";
        cout << endl;

        displayHP(player_hp, boss_hp, boss_name, player_max_hp, boss_max_hp);

        if (boss_charging) {
            cout << ">> WARNING: " << boss_name << " is CHARGED UP -- next hit is massive!" << endl;
        }

        // ---- MENU ----
        cout << endl << "What do you do?" << endl;
        cout << "1. Basic Attack       (deals ~" << player_strength << " dmg, free)" << endl;
        cout << "2. " << ability1_name
             << "  (deals ~" << (int)(player_strength * 1.6f * multiplier)
             << " dmg, " << ability1_cost << " MP)" << endl;
        cout << "3. " << ability2_name
             << "  (deals ~" << (int)(player_strength * 2.5f * multiplier)
             << " dmg, " << ability2_cost << " MP)" << endl;
        cout << "4. " << ability3_name
             << "  (" << ability3_cost << " MP - special)" << endl;
        cout << "5. Defend             (free -- reduce next hit by 60%)" << endl;
        cout << "6. Use Potion         (restore 40 HP)" << endl;
        cout << "7. Use Ether          (restore 30 MP)" << endl;
        cout << "8. Inventory" << endl;
        cout << "9. Flee               (cost: 1 Potion -- retry pre-battle)" << endl;

        int choice = getPlayerChoice(9);

        int damage_dealt = 0;
        dodging   = false;
        shielded  = false;
        defending = false;

        // ---- PLAYER ACTION ----
        if (choice == 1) {
            damage_dealt = player_strength + (rand() % 5);
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
                cout << "!! Not enough mana! (" << ability1_cost << " MP needed, have " << player_mana << ")" << endl;
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
                if (multiplier > 1.0f) cout << "CRITICAL ADVANTAGE -- the boss staggers!" << endl;
                if (multiplier < 1.0f) cout << "The boss absorbs most of it..." << endl;
            } else {
                cout << "!! Not enough mana! (" << ability2_cost << " MP needed, have " << player_mana << ")" << endl;
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
                    cout << "You dissolve into wind -- you will dodge the next attack!" << endl;
                } else if (elemental_name == "Water") {
                    int heal = 30 + (rand() % 15);
                    player_hp = min(player_hp + heal, player_max_hp);
                    cout << "Healing Current flows -- restored " << heal << " HP! ["
                         << player_hp << "/" << player_max_hp << "]" << endl;
                }
            } else {
                cout << "!! Not enough mana! (" << ability3_cost << " MP needed, have " << player_mana << ")" << endl;
            }

        } else if (choice == 5) {
            // ---- DEFEND ----
            defending = true;
            cout << "You brace yourself — incoming damage reduced by 60%!" << endl;

        } else if (choice == 6) {
            usePotion();
        } else if (choice == 7) {
            useEther();
        } else if (choice == 8) {
            displayInventory();
        } else if (choice == 9) {
            // ---- FLEE ----
            if (potions > 0) {
                potions--;
                cout << endl;
                cout << "You slam a smoke bomb — spending a potion to create cover." << endl;
                cout << "The " << boss_name << " roars as you slip away." << endl;
                cout << "You regroup. But the titan is still out there." << endl;
                cout << endl;
                // Restart pre-battle (boss_hp partially restored — it healed while you fled)
                int recovered_hp = (int)(boss_max_hp * 0.15f);
                boss_hp = min(boss_hp + recovered_hp, boss_max_hp);
                cout << ">> The " << boss_name << " recovered " << recovered_hp << " HP while you fled." << endl;
                // Return to pre-battle, then re-enter the fight
                choice_pre_battle(boss_name, boss_element, boss_hp, boss_strength, boss_num);
                return; // Exit this battle instance — new one started in pre-battle
            } else {
                cout << "!! No potions left — you can't afford to flee!" << endl;
                cout << "   You have to fight!" << endl;
            }
        }

        // Track and apply player damage
        boss_hp -= damage_dealt;
        total_damage_dealt += damage_dealt;
        if (boss_hp < 0) boss_hp = 0;

        // ---- BOSS TURN ----
        if (boss_hp > 0) {
            cout << endl;

            if (stunned) {
                cout << boss_name << " sparks and stutters -- STUNNED! Skipping their turn!" << endl;
                stunned = false;
            } else {
                // ---- BOSS MOVE SELECTION ----
                int boss_move;
                if (in_rage) {
                    int roll = rand() % 10;
                    if (roll < 4)      boss_move = 1;
                    else if (roll < 7) boss_move = 2;
                    else               boss_move = 4; // rage special
                } else {
                    int roll = rand() % 10;
                    if (roll < 4)      boss_move = 1;
                    else if (roll < 7) boss_move = 2;
                    else               boss_move = 3; // charge up
                }

                // Helper lambda-style: apply damage with defend/dodge/shield modifiers
                // Written as a local function simulation using inline checks
                auto apply_hit = [&](int raw_dmg, string move_name) {
                    if (dodging) {
                        cout << "You dodge " << move_name << " completely!" << endl;
                        return;
                    }
                    if (defending) raw_dmg = (int)(raw_dmg * 0.4f); // 60% reduction
                    if (shielded)  raw_dmg /= 2;
                    raw_dmg = max(raw_dmg, 1);
                    player_hp -= raw_dmg;
                    total_damage_taken += raw_dmg;
                    if (player_hp < 0) player_hp = 0;

                    if (defending) cout << "You brace — ";
                    if (shielded)  cout << "Shield halves it — ";
                    cout << boss_name << "'s " << move_name << " hits for " << raw_dmg << " damage!" << endl;
                };

                if (boss_move == 1) {
                    // HEAVY STRIKE
                    int boss_dmg = (int)(boss_strength * 1.3f)
                                 + (rand() % (BOSS_MAX_ATTACK - BOSS_MIN_ATTACK + 1));
                    bool boss_crit = (rand() % 100 < 15);
                    if (boss_crit) {
                        boss_dmg = (int)(boss_dmg * 2.0f);
                        cout << "** BOSS CRITICAL INCOMING! **" << endl;
                    }
                    if (boss_charging) {
                        boss_dmg = (int)(boss_dmg * 1.5f); // charged hit bonus
                        boss_charging = false;
                        cout << boss_name << " RELEASES the charge into a HEAVY STRIKE!" << endl;
                    } else {
                        cout << boss_name << " winds up a HEAVY STRIKE!" << endl;
                    }
                    apply_hit(boss_dmg, "Heavy Strike");

                } else if (boss_move == 2) {
                    // QUICK JAB x2
                    int jab1 = (int)(boss_strength * 0.6f) + (rand() % 5);
                    int jab2 = (int)(boss_strength * 0.6f) + (rand() % 5);
                    cout << boss_name << " strikes TWICE in rapid succession!" << endl;

                    if (dodging) {
                        cout << "  Jab 1: You dodge!" << endl;
                        // Second jab still lands — can't dodge twice
                        int reduced = defending ? (int)(jab2 * 0.4f) : (shielded ? jab2 / 2 : jab2);
                        reduced = max(reduced, 1);
                        player_hp -= reduced;
                        total_damage_taken += reduced;
                        cout << "  Jab 2: Can't dodge again -- " << reduced << " damage!" << endl;
                    } else {
                        if (defending) { jab1 = (int)(jab1 * 0.4f); jab2 = (int)(jab2 * 0.4f); }
                        if (shielded)  { jab1 /= 2; jab2 /= 2; }
                        jab1 = max(jab1, 1); jab2 = max(jab2, 1);
                        player_hp -= jab1;
                        player_hp -= jab2;
                        total_damage_taken += jab1 + jab2;
                        cout << "  Jab 1: " << jab1 << " damage!" << endl;
                        cout << "  Jab 2: " << jab2 << " damage!" << endl;
                        cout << "  Total: " << (jab1 + jab2) << " damage!" << endl;
                    }

                } else if (boss_move == 3) {
                    // CHARGE UP (normal phase only)
                    if (!boss_charging) {
                        boss_charging = true;
                        cout << boss_name << " pulls back and begins to CHARGE..." << endl;
                        cout << ">> Its next attack will deal +50% damage!" << endl;
                    } else {
                        int charged_dmg = boss_strength * 2 + (rand() % 10);
                        boss_charging = false;
                        cout << boss_name << " RELEASES the charged energy!" << endl;
                        apply_hit(charged_dmg, "Charged Blast");
                    }

                } else if (boss_move == 4) {
                    // RAGE SPECIAL
                    cout << endl;
                    printDivider('!', 46);
                    cout << "!! " << boss_name << " RAGE ABILITY !!" << endl;
                    printDivider('!', 46);

                    // For Void Colossus: pick a random rage ability from all four elements
                    string rage_element = boss_element;
                    if (boss_element == "Void") {
                        int pick = rand() % 4;
                        string rage_pool[4] = {"Water", "Air", "Lightning", "Fire"};
                        rage_element = rage_pool[pick];
                        cout << "The Void channels " << rage_element << " energy!" << endl;
                    }

                    int rage_dmg = 0;

                    if (rage_element == "Water") {
                        rage_dmg = boss_strength * 2 + (rand() % 12);
                        int boss_heal = 15 + (rand() % 10);
                        boss_hp = min(boss_hp + boss_heal, boss_max_hp);
                        cout << "TIDAL SURGE: A wall of water crashes over you!" << endl;
                        cout << "The titan absorbs moisture and heals " << boss_heal << " HP!" << endl;
                        apply_hit(rage_dmg, "Tidal Surge");

                    } else if (rage_element == "Air") {
                        int h1 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        int h2 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        int h3 = (int)(boss_strength * 0.8f) + (rand() % 5);
                        cout << "CYCLONE SHRED: Three wind blades tear through you!" << endl;
                        if (dodging) {
                            cout << "You slip through the gaps -- all three miss!" << endl;
                        } else {
                            int total = h1 + h2 + h3;
                            if (defending) total = (int)(total * 0.4f);
                            if (shielded)  total /= 2;
                            total = max(total, 1);
                            player_hp -= total;
                            total_damage_taken += total;
                            cout << "  Blade 1: " << h1 << " | Blade 2: " << h2
                                 << " | Blade 3: " << h3 << endl;
                            cout << "  Total damage: " << total << endl;
                        }

                    } else if (rage_element == "Lightning") {
                        rage_dmg = boss_strength + (rand() % 8);
                        cout << "STATIC PRISON: Electricity locks your body in place!" << endl;
                        if (!dodging) {
                            apply_hit(rage_dmg, "Static Prison");
                            if (player_hp > 0) {
                                int free_hit = boss_strength + (rand() % 5);
                                cout << "You're PARALYZED -- ";
                                apply_hit(free_hit, "free strike");
                            }
                        } else {
                            cout << "Your momentum carries you out of the field!" << endl;
                        }

                    } else if (rage_element == "Fire") {
                        rage_dmg = boss_strength * 2 + (rand() % 15);
                        cout << "MAGMA ERUPTION: The ground bursts with lava!" << endl;
                        if (!dodging) {
                            apply_hit(rage_dmg, "Magma Eruption");
                            if (player_hp > 0) {
                                int burn = 10 + (rand() % 5);
                                cout << "You're BURNING! ";
                                apply_hit(burn, "burn");
                            }
                        } else {
                            cout << "You launch off the ground before the lava hits!" << endl;
                        }
                    }
                }
            } // end stunned check
        } // end boss turn

        if (player_hp < 0) player_hp = 0;
        turn++;

    } // end battle loop

    // ---- OUTCOME ----
    cout << endl;
    if (player_hp <= 0) {
        printHeader("YOU HAVE FALLEN");
        cout << endl;
        cout << boss_name << " lets out a horrific roar as darkness consumes the land." << endl;
        cout << "The world was not saved. The apocalypse is complete." << endl;
        cout << endl;
        cout << "  Damage dealt  : " << total_damage_dealt << endl;
        cout << "  Damage taken  : " << total_damage_taken << endl;
        cout << "  Bosses slain  : " << battles_won << endl;
        cout << endl;
    } else {
        battles_won++;
        printHeader("TITAN DEFEATED");
        cout << endl;
        cout << boss_name << " collapses with a world-shaking crash!" << endl;
        cout << "The corruption shatters. Light breaks through the clouds." << endl;
        cout << "You stand victorious. HP remaining: " << player_hp << "/" << player_max_hp << endl;
        cout << endl;

        // Advance campaign
        campaign_next_boss(boss_num);
    }
}

#endif