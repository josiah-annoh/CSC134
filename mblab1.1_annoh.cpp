/*
M6LAB1 - Slime Roulette (Advanced)
annohj
4/15/26

New features:
  - Player vs Dealer with alternating turns
  - Health points (4 HP each, slime costs 1 HP)
  - Item system: Shield, Medkit, Peek, Double Shot, Skip
  - 3 rounds with escalating magazine sizes
  - Win/loss/tie tracking across rounds
  - Dealer AI with basic decision-making
*/

#include <iostream>
#include <vector>
#include <algorithm>   // std::shuffle
#include <random>      // std::mt19937, std::uniform_int_distribution
#include <string>

using namespace std;

// ─── Constants ───────────────────────────────────────────────────────────────
const int MAX_HP    = 4;
const int NUM_ROUNDS = 3;

// ─── Item names ──────────────────────────────────────────────────────────────
// Each item is stored as a char code for simplicity
// 'H' = Shield  'M' = Medkit  'P' = Peek  'D' = Double Shot  'K' = Skip
const vector<char> ITEM_POOL = {'H', 'M', 'P', 'D', 'K'};

string itemName(char item) {
    switch (item) {
        case 'H': return "Shield     (block next slime hit)";
        case 'M': return "Medkit     (restore 1 HP)";
        case 'P': return "Peek       (see next shell)";
        case 'D': return "Double Shot(fire twice this turn)";
        case 'K': return "Skip       (opponent loses next turn)";
        default:  return "Unknown";
    }
}

// ─── Function Prototypes ─────────────────────────────────────────────────────
void  loadMagazine(vector<char>& mag, int slime, int water);
void  displayMagazine(const vector<char>& mag);
char  fireShot(vector<char>& mag);
void  displayStatus(int playerHP, int dealerHP, int round);
char  randomItem();
void  offerItem(vector<char>& inventory);
bool  applyItem(vector<char>& inventory,
                vector<char>& mag,
                int& selfHP, int& opponentHP,
                bool& shielded, bool& opponentSkipped,
                bool& doubleShotActive);
void  takeTurn(const string& name,
               vector<char>& mag,
               int& selfHP, int& opponentHP,
               bool& shielded, bool& opponentSkipped,
               vector<char>& inventory,
               bool isPlayer);
bool  playRound(int roundNum, int& playerHP, int& dealerHP);

// ─── Main ────────────────────────────────────────────────────────────────────
int main() {
    cout << "========================================\n";
    cout << "         SLIME ROULETTE\n";
    cout << "========================================\n";
    cout << "Rules:\n";
    cout << "  - Slime shell (S) = -1 HP\n";
    cout << "  - Water shell (W) = safe\n";
    cout << "  - First to 0 HP loses the round\n";
    cout << "  - Best of " << NUM_ROUNDS << " rounds wins!\n";
    cout << "========================================\n\n";

    int playerHP = MAX_HP;
    int dealerHP = MAX_HP;
    int playerWins = 0;
    int dealerWins = 0;

    for (int r = 1; r <= NUM_ROUNDS; r++) {
        // Reset HP each round
        playerHP = MAX_HP;
        dealerHP = MAX_HP;

        cout << "\n>>> ROUND " << r << " of " << NUM_ROUNDS << " <<<\n";
        bool playerWonRound = playRound(r, playerHP, dealerHP);

        if (playerWonRound) {
            playerWins++;
            cout << "\n[Round " << r << "] You win this round!\n";
        } else {
            dealerWins++;
            cout << "\n[Round " << r << "] Dealer wins this round!\n";
        }

        cout << "Score -> You: " << playerWins
             << "  Dealer: " << dealerWins << "\n";
    }

    // Overall result
    cout << "\n========================================\n";
    if (playerWins > dealerWins)
        cout << "  GAME OVER — YOU WIN! Congratulations!\n";
    else if (dealerWins > playerWins)
        cout << "  GAME OVER — DEALER WINS! Better luck next time.\n";
    else
        cout << "  GAME OVER — IT'S A TIE!\n";
    cout << "  Final: You " << playerWins
         << " - " << dealerWins << " Dealer\n";
    cout << "========================================\n";

    return 0;
}

// ─── loadMagazine ────────────────────────────────────────────────────────────
// Fills the magazine with slime/water shells, then shuffles.
// Scales shell count by round so later rounds are bigger.
void loadMagazine(vector<char>& mag, int slime, int water) {
    mag.clear();
    for (int i = 0; i < slime; i++) mag.push_back('S');
    for (int i = 0; i < water; i++) mag.push_back('W');

    random_device rd;
    mt19937 gen(rd());
    shuffle(mag.begin(), mag.end(), gen);
}

// ─── displayMagazine ─────────────────────────────────────────────────────────
// Shows how many shells remain (without revealing order).
void displayMagazine(const vector<char>& mag) {
    int s = 0, w = 0;
    for (char shell : mag) {
        if (shell == 'S') s++;
        else              w++;
    }
    cout << "  Magazine: " << mag.size() << " remaining ("
         << s << " slime, " << w << " water)\n";
}

// ─── fireShot ────────────────────────────────────────────────────────────────
// Removes and returns the next shell from the back of the magazine.
char fireShot(vector<char>& mag) {
    char shell = mag.back();
    mag.pop_back();
    return shell;
}

// ─── displayStatus ───────────────────────────────────────────────────────────
// Prints a tidy header with current HP and round info.
void displayStatus(int playerHP, int dealerHP, int round) {
    cout << "\n--- Round " << round << " | "
         << "Your HP: " << playerHP << "/" << MAX_HP << "  "
         << "Dealer HP: " << dealerHP << "/" << MAX_HP << " ---\n";
}

// ─── randomItem ──────────────────────────────────────────────────────────────
// Returns a random item code from ITEM_POOL.
char randomItem() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, (int)ITEM_POOL.size() - 1);
    return ITEM_POOL[dist(gen)];
}

// ─── offerItem ───────────────────────────────────────────────────────────────
// 40% chance at the start of a player turn to receive a random item.
void offerItem(vector<char>& inventory) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);
    if (dist(gen) <= 40) {
        char gained = randomItem();
        inventory.push_back(gained);
        cout << "  [ITEM] You found: " << itemName(gained) << "!\n";
    }
}

// ─── applyItem ───────────────────────────────────────────────────────────────
// Prompts the player to use an item, returns true if one was used.
// Modifies game state via references.
bool applyItem(vector<char>& inventory,
               vector<char>& mag,
               int& selfHP, int& opponentHP,
               bool& shielded, bool& opponentSkipped,
               bool& doubleShotActive) {
    if (inventory.empty()) return false;

    cout << "\n  Your items:\n";
    for (int i = 0; i < (int)inventory.size(); i++)
        cout << "    [" << i + 1 << "] " << itemName(inventory[i]) << "\n";
    cout << "    [0] Skip (use no item)\n";
    cout << "  Choice: ";

    int choice;
    cin >> choice;

    if (choice <= 0 || choice > (int)inventory.size()) return false;

    char used = inventory[choice - 1];
    inventory.erase(inventory.begin() + choice - 1);

    switch (used) {
        case 'H':
            shielded = true;
            cout << "  Shield activated! Next slime hit blocked.\n";
            break;
        case 'M':
            if (selfHP < MAX_HP) {
                selfHP++;
                cout << "  Medkit used! HP restored to " << selfHP << ".\n";
            } else {
                cout << "  Already at full HP — medkit wasted.\n";
            }
            break;
        case 'P':
            if (!mag.empty())
                cout << "  Peek: next shell is [" << mag.back() << "]\n";
            else
                cout << "  Peek: magazine is empty!\n";
            break;
        case 'D':
            doubleShotActive = true;
            cout << "  Double Shot ready — you'll fire twice!\n";
            break;
        case 'K':
            opponentSkipped = true;
            cout << "  Skip used! Dealer loses their next turn.\n";
            break;
    }
    return true;
}

// ─── takeTurn ────────────────────────────────────────────────────────────────
// Handles one complete turn for either the player or dealer.
// Player: offered item, then prompted to fire.
// Dealer: simple AI — fires immediately, uses shield if 2+ slime shells remain.
void takeTurn(const string& name,
              vector<char>& mag,
              int& selfHP, int& opponentHP,
              bool& shielded, bool& opponentSkipped,
              vector<char>& inventory,
              bool isPlayer) {
    cout << "\n*** " << name << "'s turn ***\n";

    if (mag.empty()) {
        cout << "  Magazine empty — nothing to fire.\n";
        return;
    }

    // Item phase (player only)
    bool doubleShotActive = false;
    if (isPlayer) {
        offerItem(inventory);
        if (!inventory.empty()) {
            cout << "  Use an item? ";
            applyItem(inventory, mag, selfHP, opponentHP,
                      shielded, opponentSkipped, doubleShotActive);
        }
        cout << "  Press Enter to fire...";
        cin.ignore();
        cin.get();
    } else {
        // Dealer AI: equip shield if there are 2+ slime shells in magazine
        int slimeCount = 0;
        for (char c : mag) if (c == 'S') slimeCount++;
        if (slimeCount >= 2 && !shielded) {
            shielded = true;
            cout << "  [Dealer] Braces for impact (Shield).\n";
        }
    }

    // Fire (once, or twice if Double Shot active)
    int shots = doubleShotActive ? 2 : 1;
    for (int i = 0; i < shots && !mag.empty(); i++) {
        char shell = fireShot(mag);
        cout << "  " << name << " fires: [" << shell << "] — ";

        if (shell == 'S') {
            if (shielded) {
                cout << "BLOCKED by shield!\n";
                shielded = false;
            } else {
                opponentHP--;
                cout << "SLIME! Opponent takes 1 damage. (HP: "
                     << opponentHP << ")\n";
            }
        } else {
            cout << "water. Safe.\n";
        }
    }

    displayMagazine(mag);
}

// ─── playRound ───────────────────────────────────────────────────────────────
// Runs a full round. Round number affects shell counts.
// Returns true if player wins, false if dealer wins.
bool playRound(int roundNum, int& playerHP, int& dealerHP) {
    // Scale shells with round (round 1: 3S/2W, round 2: 4S/3W, round 3: 5S/4W)
    int slimeShells = 2 + roundNum;
    int waterShells = 1 + roundNum;

    vector<char> magazine;
    loadMagazine(magazine, slimeShells, waterShells);

    cout << "\nLoading magazine for round " << roundNum << ":\n";
    displayMagazine(magazine);

    // Per-player state
    bool playerShielded     = false;
    bool dealerShielded     = false;
    bool playerSkipped      = false;
    bool dealerSkipped      = false;
    vector<char> playerItems;

    bool playerTurn = true;   // player always goes first

    while (playerHP > 0 && dealerHP > 0) {
        // Reload if magazine runs out mid-round
        if (magazine.empty()) {
            cout << "\n  [Magazine empty — reloading for round "
                 << roundNum << "!]\n";
            loadMagazine(magazine, slimeShells, waterShells);
            displayMagazine(magazine);
        }

        displayStatus(playerHP, dealerHP, roundNum);

        if (playerTurn) {
            if (playerSkipped) {
                cout << "\n*** Your turn — SKIPPED! ***\n";
                playerSkipped = false;
            } else {
                takeTurn("You", magazine, playerHP, dealerHP,
                         playerShielded, dealerSkipped,
                         playerItems, true);
            }
        } else {
            if (dealerSkipped) {
                cout << "\n*** Dealer's turn — SKIPPED! ***\n";
                dealerSkipped = false;
            } else {
                takeTurn("Dealer", magazine, dealerHP, playerHP,
                         dealerShielded, playerSkipped,
                         playerItems, false);  // dealer has no item inventory
            }
        }

        playerTurn = !playerTurn;
    }

    return playerHP > 0;   // true = player survived
}