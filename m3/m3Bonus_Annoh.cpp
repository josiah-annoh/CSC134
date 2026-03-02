#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// ---- PROTOTYPES ----
void Fire();
void Lightning();
void Air();
void Water();
void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength);
void displayHP(int playerHP, int bossHP, string bossName, int playerMaxHP, int bossMaxHP);
void displayInventory();
void usePotion();
float getMatchupMultiplier(string attacker, string defender);

// ---- GLOBAL VARS ----
string elemental_name;
int player_strength;
int player_hp;
int player_max_hp;
int player_mana;
int player_max_mana;
int potions;
int ether;           // mana potions
int ability1_cost;
int ability2_cost;
int ability3_cost;
string ability1_name;
string ability2_name;
string ability3_name;

// ---- BALANCE CONSTANTS ----
// Boss attack range (random between these)
const int BOSS_MIN_ATTACK = 8;
const int BOSS_MAX_ATTACK = 15;

// Base strengths (tightened range for balance)
int fire_strength      = 12;
int lightning_strength = 11;
int air_strength       = 10;
int water_strength     = 11;

// Base HP
int fire_hp      = 120;
int lightning_hp = 110;
int air_hp       = 100;
int water_hp     = 115;

// Base mana
int fire_mana      = 80;
int lightning_mana = 100;
int air_mana       = 90;
int water_mana     = 90;

// ---- MATCHUP MULTIPLIER ----
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

// ---- DISPLAY HP BAR ----
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

// ---- INVENTORY ----
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

// ---- MAIN ----
int main() {
    srand(time(0)); // seed random

    string choice;

    cout << "==========================================" << endl;
    cout << "          ELEMENTAL APOCALYPSE            " << endl;
    cout << "==========================================" << endl << endl;
    cout << "The ancient seals have broken." << endl;
    cout << "Four corrupted Elemental Titans have risen, each one a force of" << endl;
    cout << "pure destruction tearing the world apart piece by piece." << endl;
    cout << "You are the last Elemental Guardian." << endl;
    cout << "Choose your power. Choose your fate." << endl << endl;

    cout << "Choose Your Element:" << endl;
    cout << "1. Fire      - High attack, lower HP. Aggressive playstyle." << endl;
    cout << "2. Lightning - Balanced attack, high mana. Speed-based abilities." << endl;
    cout << "3. Air       - Lower attack, highest dodge potential. Tricky playstyle." << endl;
    cout << "4. Water     - Balanced all around. Strong sustain and healing." << endl << endl;

    cout << "> ";
    cin >> choice;

    if ("Fire" == choice) {
        Fire();
    } else if ("Lightning" == choice) {
        Lightning();
    } else if ("Air" == choice) {
        Air();
    } else if ("Water" == choice) {
        Water();
    } else {
        cout << "The elements did not hear your call..." << endl;
    }

    cout << endl << "======== GAME OVER ========" << endl;
    return 0;
}

// ---- ELEMENT FUNCTIONS ----
void Fire() {
    elemental_name = "Fire";
    player_strength = fire_strength;
    player_hp = player_max_hp = fire_hp;
    player_mana = player_max_mana = fire_mana;
    potions = 3;
    ether = 2;

    // Fire toolkit
    ability1_name = "Flame Surge";    ability1_cost = 15; // strong single hit
    ability2_name = "Inferno Burst";  ability2_cost = 25; // massive hit
    ability3_name = "Ember Shield";   ability3_cost = 20; // reduce damage

    cout << endl << "Flames surge through your veins." << endl;
    cout << "The ground chars with every step as you march toward the corruption." << endl;
    cout << "Ahead, a colossal Tidal Titan rises from a flooded crater." << endl;
    cout << "Its body is a living ocean — and it has your scent." << endl << endl;

    start_battle("Tidal Titan", "Water", 180, 13);
}

void Lightning() {
    elemental_name = "Lightning";
    player_strength = lightning_strength;
    player_hp = player_max_hp = lightning_hp;
    player_mana = player_max_mana = lightning_mana;
    potions = 3;
    ether = 3;

    // Lightning toolkit
    ability1_name = "Volt Strike";    ability1_cost = 12; // fast reliable hit
    ability2_name = "Thunder Clap";   ability2_cost = 28; // massive AOE hit
    ability3_name = "Static Field";   ability3_cost = 18; // stun, boss loses a turn

    cout << endl << "Lightning crackles across your skin." << endl;
    cout << "You move like a bolt, crossing the shattered skybridge in seconds." << endl;
    cout << "At the edge of the world, a Cyclone Titan screams into existence." << endl;
    cout << "Winds howl so loud the ground itself cracks beneath you." << endl << endl;

    start_battle("Cyclone Titan", "Air", 165, 12);
}

void Air() {
    elemental_name = "Air";
    player_strength = air_strength;
    player_hp = player_max_hp = air_hp;
    player_mana = player_max_mana = air_mana;
    potions = 3;
    ether = 2;

    // Air toolkit
    ability1_name = "Wind Slash";     ability1_cost = 10; // cheap reliable hit
    ability2_name = "Vacuum Vortex";  ability2_cost = 22; // strong hit + dodge next attack
    ability3_name = "Gale Dodge";     ability3_cost = 15; // guaranteed dodge next hit

    cout << endl << "You dissolve into the wind." << endl;
    cout << "Invisible and weightless, you drift through the ruined canyon." << endl;
    cout << "A Storm Titan crackles to life ahead, electricity arcing from its body." << endl;
    cout << "The air around it smells like burning metal." << endl << endl;

    start_battle("Storm Titan", "Lightning", 155, 11);
}

void Water() {
    elemental_name = "Water";
    player_strength = water_strength;
    player_hp = player_max_hp = water_hp;
    player_mana = player_max_mana = water_mana;
    potions = 4;
    ether = 2;

    // Water toolkit
    ability1_name = "Tidal Wave";     ability1_cost = 15; // solid damage
    ability2_name = "Tsunami Crush";  ability2_cost = 26; // massive damage
    ability3_name = "Healing Current";ability3_cost = 20; // heal self mid battle

    cout << endl << "A wave crashes over you and recedes." << endl;
    cout << "Tides pulse through your body as you surge through the burning forest." << endl;
    cout << "At the end of the trail, a Magma Titan erupts from the earth." << endl;
    cout << "Lava drips from its fists. The trees around it are already ash." << endl << endl;

    start_battle("Magma Titan", "Fire", 175, 13);
}

// ---- BATTLE SYSTEM ----
void start_battle(string boss_name, string boss_element, int boss_hp, int boss_strength) {

    int boss_max_hp = boss_hp;
    float multiplier = getMatchupMultiplier(elemental_name, boss_element);
    bool stunned = false;       // boss stun flag
    bool dodging = false;       // player dodge flag
    bool shielded = false;      // player shield flag
    int turn = 1;

    cout << "---- MATCHUP ----" << endl;
    if (multiplier > 1.0f)
        cout << ">> ADVANTAGE: Your " << elemental_name << " is strong vs " << boss_element << "!" << endl;
    else if (multiplier < 1.0f)
        cout << ">> DISADVANTAGE: Your " << elemental_name << " is weak vs " << boss_element << "." << endl;
    else
        cout << ">> NEUTRAL matchup. Neither side has the edge." << endl;
    cout << endl;

    // ---- BATTLE LOOP ----
    while (player_hp > 0 && boss_hp > 0) {

        cout << endl << "-- Turn " << turn << " --" << endl;
        displayHP(player_hp, boss_hp, boss_name, player_max_hp, boss_max_hp);

        cout << endl << "What do you do?" << endl;
        cout << "1. Basic Attack          (deals " << player_strength << " dmg, free)" << endl;
        cout << "2. " << ability1_name << "         (deals " << (int)(player_strength * 1.6f * multiplier) << " dmg, " << ability1_cost << " MP)" << endl;
        cout << "3. " << ability2_name << "       (deals " << (int)(player_strength * 2.5f * multiplier) << " dmg, " << ability2_cost << " MP)" << endl;
        cout << "4. " << ability3_name << "        (" << ability3_cost << " MP - special effect)" << endl;
        cout << "5. Use Potion            (restore 40 HP)" << endl;
        cout << "6. Use Ether             (restore 30 MP)" << endl;
        cout << "7. Inventory" << endl;

        int choice;
        cout << "> ";
        cin >> choice;

        int damage_dealt = 0;
        dodging = false;
        shielded = false;

        if (choice == 1) {
            // Basic attack — always works, no mana
            damage_dealt = player_strength + (rand() % 5); // small random variance
            cout << "You strike with a basic attack for " << damage_dealt << " damage!" << endl;

        } else if (choice == 2) {
            // Ability 1 - strong hit
            if (player_mana >= ability1_cost) {
                player_mana -= ability1_cost;
                damage_dealt = (int)(player_strength * 1.6f * multiplier) + (rand() % 6);
                cout << "You unleash " << ability1_name << " for " << damage_dealt << " damage!" << endl;
                if (multiplier > 1.0f) cout << "Its super effective!" << endl;
                if (multiplier < 1.0f) cout << "The boss resists your element..." << endl;
            } else {
                cout << "Not enough mana! (" << ability1_cost << " MP needed)" << endl;
            }

        } else if (choice == 3) {
            // Ability 2 - massive hit
            if (player_mana >= ability2_cost) {
                player_mana -= ability2_cost;
                damage_dealt = (int)(player_strength * 2.5f * multiplier) + (rand() % 8);
                cout << "You UNLEASH " << ability2_name << " for " << damage_dealt << " damage!" << endl;
                if (multiplier > 1.0f) cout << "CRITICAL ADVANTAGE — the boss staggers!" << endl;
                if (multiplier < 1.0f) cout << "The boss absorbs most of it... " << endl;
            } else {
                cout << "Not enough mana! (" << ability2_cost << " MP needed)" << endl;
            }

        } else if (choice == 4) {
            // Ability 3 - special per element
            if (player_mana >= ability3_cost) {
                player_mana -= ability3_cost;

                if (elemental_name == "Fire") {
                    // Ember Shield - halve next boss hit
                    shielded = true;
                    cout << "Ember Shield activated! Next hit reduced by half!" << endl;
                } else if (elemental_name == "Lightning") {
                    // Static Field - stun boss, they lose next turn
                    stunned = true;
                    cout << "Static Field crackles out! " << boss_name << " is STUNNED!" << endl;
                } else if (elemental_name == "Air") {
                    // Gale Dodge - guaranteed dodge
                    dodging = true;
                    cout << "You dissolve into wind — you will dodge the next attack!" << endl;
                } else if (elemental_name == "Water") {
                    // Healing Current - heal self
                    int heal = 30 + (rand() % 15);
                    player_hp += heal;
                    if (player_hp > player_max_hp) player_hp = player_max_hp;
                    cout << "Healing Current flows through you — restored " << heal << " HP!" << endl;
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
        } else {
            cout << "You hesitate... nothing happens." << endl;
        }

        // Apply damage to boss
        boss_hp -= damage_dealt;
        if (boss_hp < 0) boss_hp = 0;

        // ---- BOSS TURN ----
        if (boss_hp > 0) {
            cout << endl;
            if (stunned) {
                cout << boss_name << " sparks and stutters — STUNNED! Skipping their turn!" << endl;
                stunned = false;
            } else {
                // Boss attacks with random variance
                int boss_dmg = boss_strength + (rand() % (BOSS_MAX_ATTACK - BOSS_MIN_ATTACK + 1));

                if (dodging) {
                    cout << "You vanish into the wind — " << boss_name << "'s attack misses completely!" << endl;
                } else if (shielded) {
                    boss_dmg /= 2;
                    player_hp -= boss_dmg;
                    cout << boss_name << " attacks but your shield absorbs half — " << boss_dmg << " damage taken!" << endl;
                } else {
                    player_hp -= boss_dmg;
                    cout << boss_name << " retaliates for " << boss_dmg << " damage!" << endl;
                }

                // Boss special move every 4 turns
                if (turn % 4 == 0) {
                    int special_dmg = boss_strength * 2;
                    if (!dodging) {
                        player_hp -= special_dmg;
                        cout << "!! " << boss_name << " unleashes a SPECIAL ATTACK for " << special_dmg << " damage !!" << endl;
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