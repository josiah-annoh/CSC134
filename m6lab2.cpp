/*
M6LAB1 - Slime Roulette (Advanced)
annohj
4/15/26

FEATURES ADDED:
  - Player vs. Dealer turn system
  - HP system (slime deals 1 damage, water is blank)
  - Items: Magnifying Glass, Handcuffs, Cigarette, Adrenaline
  - Dealer AI (uses known-shell logic and items)
  - Round system with dynamic slime/water scaling
  - Win/loss detection and replay prompt
*/

#include <iostream>
#include <vector>
#include <algorithm>   // std::shuffle
#include <random>      // std::mt19937, std::uniform_int_distribution
#include <string>
#include <limits>      // std::numeric_limits

using namespace std;

// ─── Constants ────────────────────────────────────────────────
const int PLAYER_MAX_HP  = 5;
const int DEALER_MAX_HP  = 5;
const int ITEMS_PER_ROUND = 2;   // items dealt to each side per round

// ─── Item Types ───────────────────────────────────────────────
enum Item {
    NONE          = 0,
    MAGNIFY_GLASS = 1,   // peek the next shell
    HANDCUFFS     = 2,   // skip dealer's next turn
    CIGARETTE     = 3,   // heal 1 HP (max 5)
    ADRENALINE    = 4    // steal and use one of dealer's items
};

string itemName(Item it) {
    switch (it) {
        case MAGNIFY_GLASS: return "Magnifying Glass";
        case HANDCUFFS:     return "Handcuffs";
        case CIGARETTE:     return "Cigarette";
        case ADRENALINE:    return "Adrenaline";
        default:            return "None";
    }
}

// ─── Game State ───────────────────────────────────────────────
struct GameState {
    vector<char> magazine;

    int playerHP   = PLAYER_MAX_HP;
    int dealerHP   = DEALER_MAX_HP;
    int round      = 1;

    vector<Item> playerItems;
    vector<Item> dealerItems;

    bool dealerCuffed     = false;  // dealer skips next turn
    bool nextShellKnown   = false;  // someone used magnifying glass
    char knownShell       = '?';    // what the peek revealed
};

// ─── Random helpers ───────────────────────────────────────────
mt19937& rng() {
    static mt19937 gen(random_device{}());
    return gen;
}

int randInt(int lo, int hi) {   // inclusive
    return uniform_int_distribution<int>(lo, hi)(rng());
}

// ─── Function Prototypes ──────────────────────────────────────
void        loadMagazine(vector<char>& mag, int slime, int water);
void        displayMagazine(const vector<char>& mag);
char        fireShot(vector<char>& mag);
void        dealItems(GameState& gs);
Item        randomItem();
void        displayStatus(const GameState& gs);
void        displayItems(const vector<Item>& items, const string& owner);
bool        useItem(GameState& gs, bool isPlayer);
void        playerTurn(GameState& gs);
void        dealerTurn(GameState& gs);
bool        checkRoundEnd(GameState& gs);
void        startRound(GameState& gs);
void        clearInput();
int         getChoice(int lo, int hi);

// ─── Main ─────────────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════╗\n"
         << "║      S L I M E  R O U L E T T E ║\n"
         << "╚══════════════════════════════════╝\n\n"
         << "Rules:\n"
         << "  Slime (S) = 1 damage to whoever it hits.\n"
         << "  Water (W) = blank — if you shoot yourself with\n"
         << "              a blank, you get an extra turn.\n"
         << "  Reduce the Dealer to 0 HP to win the round.\n"
         << "  Lose all your HP and the game is over.\n\n"
         << "Items:\n"
         << "  [1] Magnifying Glass — peek the next shell.\n"
         << "  [2] Handcuffs        — dealer skips next turn.\n"
         << "  [3] Cigarette        — heal 1 HP (max " << PLAYER_MAX_HP << ").\n"
         << "  [4] Adrenaline       — steal & use a dealer item.\n\n";

    char replay = 'y';
    while (replay == 'y' || replay == 'Y') {
        // ── Fresh game ────────────────────────────────────────
        GameState gs;
        gs.playerHP = PLAYER_MAX_HP;
        gs.dealerHP = DEALER_MAX_HP;
        gs.round    = 1;

        while (gs.playerHP > 0 && gs.dealerHP > 0) {
            startRound(gs);

            // Play out the magazine
            while (!gs.magazine.empty()
                   && gs.playerHP > 0
                   && gs.dealerHP > 0) {
                playerTurn(gs);
                if (gs.playerHP <= 0 || gs.dealerHP <= 0) break;
                dealerTurn(gs);
            }

            if (gs.playerHP <= 0 || gs.dealerHP <= 0) break;
            gs.round++;
        }

        if (gs.dealerHP <= 0 && gs.playerHP > 0) {
            cout << "\n★  You defeated the Dealer! You win!  ★\n";
        } else {
            cout << "\n✕  You were slimed out. Game over.\n";
        }

        cout << "\nPlay again? (y/n): ";
        cin  >> replay;
        clearInput();
    }

    cout << "Thanks for playing Slime Roulette!\n";
    return 0;
}

// ─── Round Setup ──────────────────────────────────────────────
void startRound(GameState& gs) {
    // Scale difficulty: more shells as rounds progress
    int slime = 2 + gs.round;
    int water = max(1, 4 - gs.round / 2);

    cout << "\n══════════════════════════════════════\n"
         << "  ROUND " << gs.round
         << "  —  Loading: " << slime << " slime, " << water << " water\n"
         << "══════════════════════════════════════\n";

    loadMagazine(gs.magazine, slime, water);
    dealItems(gs);
    gs.dealerCuffed   = false;
    gs.nextShellKnown = false;
    gs.knownShell     = '?';

    displayStatus(gs);
}

// ─── loadMagazine ─────────────────────────────────────────────
void loadMagazine(vector<char>& mag, int slime, int water) {
    mag.clear();
    for (int i = 0; i < slime; i++) mag.push_back('S');
    for (int i = 0; i < water; i++) mag.push_back('W');
    shuffle(mag.begin(), mag.end(), rng());
}

// ─── displayMagazine ──────────────────────────────────────────
void displayMagazine(const vector<char>& mag) {
    int s = 0, w = 0;
    for (char shell : mag) {
        if (shell == 'S') s++;
        else               w++;
    }
    cout << "  Magazine: " << mag.size()
         << " shells  (" << s << " slime, " << w << " water)\n";
}

// ─── fireShot ─────────────────────────────────────────────────
char fireShot(vector<char>& mag) {
    char shell = mag.back();
    mag.pop_back();
    return shell;
}

// ─── Item helpers ─────────────────────────────────────────────
Item randomItem() {
    // Weighted: magnify most common, adrenaline rarest
    int r = randInt(1, 10);
    if (r <= 3) return MAGNIFY_GLASS;
    if (r <= 5) return HANDCUFFS;
    if (r <= 8) return CIGARETTE;
    return ADRENALINE;
}

void dealItems(GameState& gs) {
    // Keep existing items (carry-over between rounds), add new ones
    for (int i = 0; i < ITEMS_PER_ROUND; i++) {
        gs.playerItems.push_back(randomItem());
        gs.dealerItems.push_back(randomItem());
    }
    cout << "  Items dealt!\n";
    displayItems(gs.playerItems, "Your");
}

void displayItems(const vector<Item>& items, const string& owner) {
    cout << "  " << owner << " items: ";
    if (items.empty()) { cout << "(none)\n"; return; }
    for (size_t i = 0; i < items.size(); i++) {
        cout << "[" << (i + 1) << "] " << itemName(items[i]);
        if (i + 1 < items.size()) cout << "  ";
    }
    cout << "\n";
}

void displayStatus(const GameState& gs) {
    cout << "\n  Player HP: " << gs.playerHP << "/" << PLAYER_MAX_HP
         << "   |   Dealer HP: " << gs.dealerHP << "/" << DEALER_MAX_HP << "\n";
    displayMagazine(gs.magazine);
    displayItems(gs.playerItems, "Your");
    if (gs.dealerCuffed) cout << "  [Dealer is CUFFED — will skip next turn]\n";
    cout << "\n";
}

// ─── useItem — player interactive ─────────────────────────────
// Returns true if an item was used (so caller can re-prompt if needed)
bool useItem(GameState& gs, bool isPlayer) {
    vector<Item>& items = isPlayer ? gs.playerItems : gs.dealerItems;
    if (items.empty()) return false;

    if (isPlayer) {
        cout << "  Choose item to use (0 = skip):\n";
        displayItems(items, "Your");
        int choice = getChoice(0, (int)items.size());
        if (choice == 0) return false;

        Item it = items[choice - 1];
        items.erase(items.begin() + choice - 1);

        switch (it) {
            case MAGNIFY_GLASS:
                if (!gs.magazine.empty()) {
                    gs.nextShellKnown = true;
                    gs.knownShell     = gs.magazine.back();
                    cout << "  → You peek: the next shell is "
                         << (gs.knownShell == 'S' ? "SLIME 🟢" : "WATER 💧")
                         << "!\n";
                } else {
                    cout << "  → Magazine is empty — nothing to peek.\n";
                }
                break;
            case HANDCUFFS:
                gs.dealerCuffed = true;
                cout << "  → You slap the cuffs on the Dealer. "
                        "They'll skip their next turn.\n";
                break;
            case CIGARETTE:
                if (gs.playerHP < PLAYER_MAX_HP) {
                    gs.playerHP++;
                    cout << "  → You smoke a cigarette and heal to "
                         << gs.playerHP << " HP.\n";
                } else {
                    cout << "  → You're already at full HP. "
                            "Wasted, but stylish.\n";
                }
                break;
            case ADRENALINE:
                if (gs.dealerItems.empty()) {
                    cout << "  → Dealer has no items to steal!\n";
                } else {
                    // Steal first dealer item and use it immediately
                    Item stolen = gs.dealerItems.front();
                    gs.dealerItems.erase(gs.dealerItems.begin());
                    cout << "  → You steal the Dealer's "
                         << itemName(stolen) << " and use it!\n";
                    gs.playerItems.insert(gs.playerItems.begin(), stolen);
                    useItem(gs, true);   // recurse to apply the stolen item
                }
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}

// ─── Dealer AI item logic ──────────────────────────────────────
void dealerUseItems(GameState& gs) {
    // Dealer uses items greedily before deciding to shoot
    bool used = true;
    while (used && !gs.dealerItems.empty()) {
        used = false;
        for (size_t i = 0; i < gs.dealerItems.size(); i++) {
            Item it = gs.dealerItems[i];

            // Heal if hurt
            if (it == CIGARETTE && gs.dealerHP < DEALER_MAX_HP) {
                gs.dealerItems.erase(gs.dealerItems.begin() + i);
                gs.dealerHP++;
                cout << "  Dealer uses a Cigarette and heals to "
                     << gs.dealerHP << " HP.\n";
                used = true; break;
            }
            // Peek if shell unknown
            if (it == MAGNIFY_GLASS && !gs.nextShellKnown
                && !gs.magazine.empty()) {
                gs.dealerItems.erase(gs.dealerItems.begin() + i);
                gs.nextShellKnown = true;
                gs.knownShell     = gs.magazine.back();
                cout << "  Dealer uses Magnifying Glass...\n";
                used = true; break;
            }
            // Cuff player if not already done
            if (it == HANDCUFFS) {
                // Dealer version: cuffs player (we repurpose the flag direction)
                // We'll handle this with a separate flag below
                gs.dealerItems.erase(gs.dealerItems.begin() + i);
                cout << "  Dealer slaps Handcuffs on you! "
                        "You'll skip your next turn.\n";
                // signal via knownShell abuse — instead we add a proper flag
                // (see GameState extension note — here we just stun for 1 turn
                //  by printing; for simplicity we skip the player's next item phase)
                gs.dealerCuffed = false;   // cuffs were on dealer; now used on player
                // We mark with a sentinel so playerTurn can check
                gs.knownShell = 'X';       // reuse field as player-stunned flag
                used = true; break;
            }
        }
    }
}

// ─── Player Turn ──────────────────────────────────────────────
void playerTurn(GameState& gs) {
    if (gs.magazine.empty()) return;

    // Check if player was cuffed by dealer (sentinel 'X')
    if (gs.knownShell == 'X') {
        cout << "\n  You're cuffed — skipping your turn.\n";
        gs.knownShell = '?';
        return;
    }

    cout << "─────────────────────────────────────\n";
    cout << "  YOUR TURN\n";
    displayStatus(gs);

    // Optional: use an item first
    if (!gs.playerItems.empty()) {
        cout << "  Use an item before shooting? ";
        useItem(gs, true);
    }

    // Choose target
    cout << "  Shoot:\n"
         << "    [1] Dealer\n"
         << "    [2] Yourself\n"
         << "  Choice: ";
    int target = getChoice(1, 2);

    char shell = fireShot(gs.magazine);
    gs.nextShellKnown = false;
    gs.knownShell     = '?';

    cout << "  *CLICK* — " << (shell == 'S' ? "SLIME! 🟢" : "WATER! 💧") << "\n";

    if (shell == 'S') {
        // Slime hits
        if (target == 1) {
            gs.dealerHP--;
            cout << "  Dealer takes 1 damage. Dealer HP: "
                 << gs.dealerHP << "\n";
        } else {
            gs.playerHP--;
            cout << "  You take 1 damage. Your HP: "
                 << gs.playerHP << "\n";
        }
    } else {
        // Blank (water)
        if (target == 2) {
            cout << "  Blank on yourself — you get an extra turn!\n";
            playerTurn(gs);   // recursive extra turn
        } else {
            cout << "  Blank hits the dealer — no damage.\n";
        }
    }
}

// ─── Dealer Turn ──────────────────────────────────────────────
void dealerTurn(GameState& gs) {
    if (gs.magazine.empty()) return;

    if (gs.dealerCuffed) {
        cout << "\n─────────────────────────────────────\n"
             << "  DEALER'S TURN — Dealer is CUFFED and skips!\n";
        gs.dealerCuffed = false;
        return;
    }

    cout << "\n─────────────────────────────────────\n"
         << "  DEALER'S TURN\n";

    dealerUseItems(gs);
    if (gs.magazine.empty()) return;

    // Dealer decision: if next shell is known, act optimally
    bool shootSelf = false;
    if (gs.nextShellKnown) {
        if (gs.knownShell == 'W') {
            shootSelf = true;   // blank on self = free extra turn
            cout << "  Dealer knows it's a blank — shoots themselves!\n";
        } else {
            cout << "  Dealer knows it's slime — shoots you!\n";
        }
        gs.nextShellKnown = false;
        gs.knownShell     = '?';
    } else {
        // Random 60/40 toward player (slight aggression bias)
        shootSelf = (randInt(1, 10) <= 4);
        cout << "  Dealer " << (shootSelf ? "shoots themselves." : "shoots you.") << "\n";
    }

    char shell = fireShot(gs.magazine);
    cout << "  *CLICK* — " << (shell == 'S' ? "SLIME! 🟢" : "WATER! 💧") << "\n";

    if (shell == 'S') {
        if (shootSelf) {
            gs.dealerHP--;
            cout << "  Dealer takes 1 damage. Dealer HP: "
                 << gs.dealerHP << "\n";
        } else {
            gs.playerHP--;
            cout << "  You take 1 damage. Your HP: "
                 << gs.playerHP << "\n";
        }
    } else {
        if (shootSelf) {
            cout << "  Blank — Dealer gets another turn!\n";
            dealerTurn(gs);   // recursive extra turn
        } else {
            cout << "  Blank — no damage to you.\n";
        }
    }
}

// ─── Input Utilities ──────────────────────────────────────────
void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getChoice(int lo, int hi) {
    int val;
    while (true) {
        cin >> val;
        if (cin.fail() || val < lo || val > hi) {
            cin.clear();
            clearInput();
            cout << "  Enter a number between " << lo
                 << " and " << hi << ": ";
        } else {
            clearInput();
            return val;
        }
    }
}