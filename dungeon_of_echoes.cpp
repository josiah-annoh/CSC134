// ============================================================
//  Dungeon of Echoes — Text Adventure / Dungeon Crawler
//  M7HW1 Mini-Project
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// ─── Utility ─────────────────────────────────────────────────
void pause() {
    cout << "\n[Press ENTER to continue...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int randInt(int lo, int hi) {          // inclusive on both ends
    return lo + rand() % (hi - lo + 1);
}

void printLine(char c = '-', int len = 50) {
    cout << string(len, c) << "\n";
}

// ─── Item ────────────────────────────────────────────────────
struct Item {
    string name;
    int attackBonus;
    int defenseBonus;

    Item(const string& n = "Nothing", int a = 0, int d = 0)
        : name(n), attackBonus(a), defenseBonus(d) {}
};

// ─── Player ──────────────────────────────────────────────────
struct Player {
    string name;
    string className;
    int hp, maxHp;
    int attack, defense;
    int gold;
    int floor;
    Item weapon;
    Item armor;
    vector<Item> inventory;

    Player() : hp(0), maxHp(0), attack(0), defense(0),
               gold(0), floor(1),
               weapon("Fists", 0, 0), armor("Rags", 0, 0) {}

    int totalAttack()  const { return attack  + weapon.attackBonus; }
    int totalDefense() const { return defense + armor.defenseBonus; }

    void printStats() const {
        printLine();
        cout << "  " << name << " the " << className << "\n";
        printLine();
        cout << "  HP:      " << hp << " / " << maxHp << "\n";
        cout << "  Attack:  " << totalAttack()
             << " (" << attack << " + " << weapon.attackBonus << " from " << weapon.name << ")\n";
        cout << "  Defense: " << totalDefense()
             << " (" << defense << " + " << armor.defenseBonus << " from " << armor.name << ")\n";
        cout << "  Gold:    " << gold << "\n";
        cout << "  Floor:   " << floor << "\n";
        printLine();
    }
};

// ─── Monster ─────────────────────────────────────────────────
struct Monster {
    string name;
    int hp, maxHp;
    int attack, defense;
    int goldReward;
    bool isBoss;

    Monster(const string& n, int h, int a, int d, int g, bool boss = false)
        : name(n), hp(h), maxHp(h), attack(a), defense(d),
          goldReward(g), isBoss(boss) {}

    void printStats() const {
        cout << "  [" << name << "]  HP: " << hp << "/" << maxHp
             << "  ATK: " << attack << "  DEF: " << defense << "\n";
    }
};

// ─── Monster factory ─────────────────────────────────────────
Monster makeMonster(int floor) {
    // Scaling: each floor monsters get slightly tougher
    int scale = floor - 1;

    vector<Monster> pool = {
        Monster("Goblin",       8  + scale*2,  4 + scale,   1,          5  + scale*2),
        Monster("Skeleton",     10 + scale*2,  5 + scale,   2 + scale,  7  + scale*2),
        Monster("Slime",        12 + scale*3,  3 + scale,   0,          4  + scale),
        Monster("Orc",          14 + scale*3,  6 + scale,   3 + scale,  10 + scale*3),
        Monster("Dark Wizard",  10 + scale*2,  8 + scale,   1,          12 + scale*3),
    };

    return pool[randInt(0, (int)pool.size() - 1)];
}

Monster makeBoss(int floor) {
    int scale = floor - 1;
    vector<Monster> bosses = {
        Monster("Stone Golem",    30 + scale*5, 8  + scale*2, 5 + scale, 30 + scale*5, true),
        Monster("Vampire Lord",   25 + scale*4, 12 + scale*2, 3 + scale, 40 + scale*5, true),
        Monster("Shadow Dragon",  40 + scale*6, 10 + scale*2, 4 + scale, 50 + scale*6, true),
    };
    return bosses[randInt(0, (int)bosses.size() - 1)];
}

// ─── Loot tables ─────────────────────────────────────────────
Item randomWeapon(int floor) {
    vector<Item> weapons = {
        Item("Rusty Sword",   2, 0),
        Item("Short Bow",     3, 0),
        Item("Iron Axe",      4, 0),
        Item("Silver Blade",  5, 0),
        Item("Enchanted Staff",6,0),
        Item("Dragon Fang",   8, 0),
    };
    int idx = min((int)weapons.size() - 1, randInt(0, floor));
    return weapons[idx];
}

Item randomArmor(int floor) {
    vector<Item> armors = {
        Item("Leather Vest",   0, 2),
        Item("Chain Mail",     0, 4),
        Item("Iron Shield",    0, 5),
        Item("Knight's Plate", 0, 7),
        Item("Mithril Coat",   0, 9),
    };
    int idx = min((int)armors.size() - 1, randInt(0, floor));
    return armors[idx];
}

// ─── Combat ──────────────────────────────────────────────────
// Returns true if player survived
bool doCombat(Player& player, Monster& monster) {
    cout << "\n";
    printLine('=');
    if (monster.isBoss)
        cout << "  *** BOSS BATTLE ***\n";
    cout << "  A " << monster.name << " appears!\n";
    printLine('=');

    while (player.hp > 0 && monster.hp > 0) {
        // Show state
        cout << "\n  YOUR HP: " << player.hp << "/" << player.maxHp
             << "   |   " << monster.name << " HP: " << monster.hp << "/" << monster.maxHp << "\n";
        cout << "\n  What do you do?\n";
        cout << "  1) Attack\n";
        cout << "  2) Use Potion (+" << (player.maxHp / 3) << " HP)\n";
        cout << "  3) Flee (50% chance)\n";
        cout << "  > ";

        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            // Player attacks
            int dmg = max(1, player.totalAttack() - monster.defense + randInt(-1, 2));
            monster.hp -= dmg;
            cout << "  You hit the " << monster.name << " for " << dmg << " damage!\n";

            if (monster.hp <= 0) break;

            // Monster attacks
            int monDmg = max(1, monster.attack - player.totalDefense() + randInt(-1, 2));
            player.hp -= monDmg;
            cout << "  The " << monster.name << " hits you for " << monDmg << " damage!\n";

        } else if (choice == 2) {
            // Potion
            int heal = player.maxHp / 3;
            player.hp = min(player.maxHp, player.hp + heal);
            cout << "  You drink a potion and recover " << heal << " HP!\n";

            // Monster still attacks
            int monDmg = max(1, monster.attack - player.totalDefense() + randInt(-1, 2));
            player.hp -= monDmg;
            cout << "  The " << monster.name << " hits you for " << monDmg << " damage!\n";

        } else if (choice == 3) {
            if (randInt(1, 2) == 1) {
                cout << "  You successfully fled!\n";
                return true;   // alive but no reward
            } else {
                cout << "  You couldn't escape!\n";
                int monDmg = max(1, monster.attack - player.totalDefense() + randInt(-1, 2));
                player.hp -= monDmg;
                cout << "  The " << monster.name << " hits you for " << monDmg << " damage!\n";
            }
        } else {
            cout << "  Invalid choice.\n";
        }
    }

    if (player.hp <= 0) {
        cout << "\n  You have been defeated...\n";
        return false;
    }

    // Victory
    cout << "\n  You defeated the " << monster.name << "!\n";
    player.gold += monster.goldReward;
    cout << "  You found " << monster.goldReward << " gold! (Total: " << player.gold << ")\n";
    return true;
}

// ─── Room events ─────────────────────────────────────────────
void eventEmpty(Player& player) {
    cout << "  The room is eerily quiet. Nothing here.\n";
}

void eventTreasure(Player& player) {
    int roll = randInt(1, 3);
    if (roll == 1) {
        Item w = randomWeapon(player.floor);
        cout << "  You found a chest containing a " << w.name << "!\n";
        cout << "  (ATK bonus: +" << w.attackBonus << ")\n";
        if (w.attackBonus > player.weapon.attackBonus) {
            cout << "  It's better than your " << player.weapon.name << ". You equip it.\n";
            player.weapon = w;
        } else {
            cout << "  It's weaker than your " << player.weapon.name << ". You leave it.\n";
        }
    } else if (roll == 2) {
        Item a = randomArmor(player.floor);
        cout << "  You found a chest containing " << a.name << "!\n";
        cout << "  (DEF bonus: +" << a.defenseBonus << ")\n";
        if (a.defenseBonus > player.armor.defenseBonus) {
            cout << "  It's better than your " << player.armor.name << ". You equip it.\n";
            player.armor = a;
        } else {
            cout << "  It's weaker than your " << player.armor.name << ". You leave it.\n";
        }
    } else {
        int gold = randInt(5, 15) * player.floor;
        player.gold += gold;
        cout << "  You found a chest with " << gold << " gold! (Total: " << player.gold << ")\n";
    }
}

void eventShop(Player& player) {
    printLine();
    cout << "  A mysterious merchant appears!\n";
    cout << "  You have " << player.gold << " gold.\n";
    printLine();

    Item heal("Healing Potion", 0, 0);
    Item w = randomWeapon(player.floor + 1);
    Item a = randomArmor(player.floor + 1);

    int healCost = 10 * player.floor;
    int wCost    = 15 * player.floor;
    int aCost    = 15 * player.floor;

    cout << "  1) Healing Potion  (+" << (player.maxHp / 3) << " HP)  — " << healCost << " gold\n";
    cout << "  2) " << w.name << " (ATK +" << w.attackBonus << ")  — " << wCost << " gold\n";
    cout << "  3) " << a.name << " (DEF +" << a.defenseBonus << ")  — " << aCost << " gold\n";
    cout << "  4) Leave\n";
    cout << "  > ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1) {
        if (player.gold >= healCost) {
            player.gold -= healCost;
            int h = player.maxHp / 3;
            player.hp = min(player.maxHp, player.hp + h);
            cout << "  You buy a potion and recover " << h << " HP!\n";
        } else {
            cout << "  Not enough gold!\n";
        }
    } else if (choice == 2) {
        if (player.gold >= wCost) {
            player.gold -= wCost;
            player.weapon = w;
            cout << "  You buy and equip the " << w.name << "!\n";
        } else {
            cout << "  Not enough gold!\n";
        }
    } else if (choice == 3) {
        if (player.gold >= aCost) {
            player.gold -= aCost;
            player.armor = a;
            cout << "  You buy and equip the " << a.name << "!\n";
        } else {
            cout << "  Not enough gold!\n";
        }
    } else {
        cout << "  You nod and move on.\n";
    }
}

void eventHealingSpring(Player& player) {
    int heal = randInt(5, 15);
    player.hp = min(player.maxHp, player.hp + heal);
    cout << "  You discover a glowing healing spring!\n";
    cout << "  You recover " << heal << " HP. (HP: " << player.hp << "/" << player.maxHp << ")\n";
}

// ─── Floor exploration ───────────────────────────────────────
// Returns false if the player died
bool exploreFloor(Player& player) {
    int rooms = randInt(3, 5);   // rooms before boss
    cout << "\n";
    printLine('=');
    cout << "  FLOOR " << player.floor << " — " << rooms << " rooms await before the boss.\n";
    printLine('=');
    pause();

    for (int r = 1; r <= rooms; r++) {
        cout << "\n";
        printLine();
        cout << "  Room " << r << " of " << rooms << "\n";
        printLine();

        // Room type weights
        int roll = randInt(1, 10);
        if (roll <= 4) {
            // Monster encounter (40%)
            Monster m = makeMonster(player.floor);
            bool survived = doCombat(player, m);
            if (!survived) return false;
        } else if (roll <= 6) {
            // Treasure (20%)
            cout << "  You enter a dusty chamber...\n";
            eventTreasure(player);
        } else if (roll <= 8) {
            // Empty (20%)
            cout << "  You step cautiously into the next room...\n";
            eventEmpty(player);
        } else if (roll == 9) {
            // Shop (10%)
            eventShop(player);
        } else {
            // Healing spring (10%)
            eventHealingSpring(player);
        }

        pause();

        // Show stats after each room
        cout << "  HP: " << player.hp << "/" << player.maxHp
             << "  |  Gold: " << player.gold
             << "  |  Weapon: " << player.weapon.name
             << "  |  Armor: " << player.armor.name << "\n";

        if (player.hp <= 0) return false;
    }

    // Boss room
    cout << "\n";
    printLine('*');
    cout << "  A massive door looms ahead. The floor trembles...\n";
    printLine('*');
    pause();

    Monster boss = makeBoss(player.floor);
    bool survived = doCombat(player, boss);
    if (!survived) return false;

    // Small HP restore between floors
    int restore = player.maxHp / 5;
    player.hp = min(player.maxHp, player.hp + restore);
    cout << "\n  You find a moment's rest. Recovered " << restore << " HP.\n";
    player.floor++;
    return true;
}

// ─── Character creation ──────────────────────────────────────
Player createPlayer() {
    Player p;
    cout << "\n  Enter your hero's name: ";
    getline(cin, p.name);
    if (p.name.empty()) p.name = "Hero";

    cout << "\n  Choose your class:\n";
    cout << "  1) Warrior  (HP: 30, ATK: 7, DEF: 4)\n";
    cout << "  2) Rogue    (HP: 22, ATK: 9, DEF: 2)\n";
    cout << "  3) Mage     (HP: 20, ATK: 11, DEF: 1)\n";
    cout << "  > ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 2:
            p.className = "Rogue";
            p.maxHp = 22; p.hp = 22;
            p.attack = 9; p.defense = 2;
            p.weapon = Item("Dagger", 2, 0);
            break;
        case 3:
            p.className = "Mage";
            p.maxHp = 20; p.hp = 20;
            p.attack = 11; p.defense = 1;
            p.weapon = Item("Spell Book", 3, 0);
            break;
        default:
            p.className = "Warrior";
            p.maxHp = 30; p.hp = 30;
            p.attack = 7; p.defense = 4;
            p.weapon = Item("Rusty Sword", 1, 0);
            break;
    }

    p.armor = Item("Leather Vest", 0, 1);
    p.gold  = 10;
    return p;
}

// ─── Main ─────────────────────────────────────────────────────
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    printLine('=');
    cout << "      DUNGEON OF ECHOES\n";
    cout << "   A Text Adventure Crawler\n";
    printLine('=');
    cout << "\n  Legend speaks of a dungeon beneath the kingdom,\n";
    cout << "  filled with monsters, treasure, and darkness.\n";
    cout << "  Many have entered. None have returned.\n";
    cout << "  Until now — perhaps.\n";
    pause();

    int totalFloors = 5;   // win condition

    while (true) {   // replay loop
        Player player = createPlayer();
        cout << "\n";
        player.printStats();
        pause();

        bool alive = true;
        while (alive && player.floor <= totalFloors) {
            alive = exploreFloor(player);
        }

        if (!alive || player.hp <= 0) {
            printLine('=');
            cout << "  GAME OVER\n";
            cout << "  " << player.name << " fell on floor " << player.floor << ".\n";
            cout << "  Gold collected: " << player.gold << "\n";
            printLine('=');
        } else {
            printLine('=');
            cout << "  *** VICTORY! ***\n";
            cout << "  " << player.name << " the " << player.className
                 << " conquered the Dungeon of Echoes!\n";
            cout << "  Gold collected: " << player.gold << "\n";
            printLine('=');
        }

        cout << "\n  Play again? (1 = Yes, 2 = No): ";
        int again;
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (again != 1) break;
    }

    cout << "\n  Thanks for playing Dungeon of Echoes!\n\n";
    return 0;
}