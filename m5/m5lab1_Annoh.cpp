/*
CSC 134
M5Lab1 - Elemental Apocalypse
m5lab1_elemental.cpp - Main program + utility function definitions
Jojo Annoh
*/

#include <iostream>
#include <string>
#include "m5lab.h"
using namespace std;

// ---- UTILITY FUNCTION PROTOTYPES ----
int getPlayerChoice(int maxChoice);
void showChoices(string c1, string c2, string c3);

// ============================================================
// ---- MAIN ----
// ============================================================

int main() {
    srand(time(0));
    game_start();
    cout << "\n======== GAME OVER ========" << endl;
    return 0;
}

// ============================================================
// ---- UTILITY FUNCTION DEFINITIONS ----
// Same pattern as the gamejam example.
// getPlayerChoice handles all input validation so every
// menu in the game is protected from bad input.
// ============================================================

int getPlayerChoice(int maxChoice) {
    int choice;
    while (true) {
        cout << "Your choice (1-" << maxChoice << "): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        if (choice >= 1 && choice <= maxChoice) {
            return choice;
        }
        cout << "Choice out of range. Pick 1-" << maxChoice << "." << endl;
    }
}

void showChoices(string c1, string c2, string c3) {
    cout << "\n--------------------------" << endl;
    cout << "1. " << c1 << endl;
    if (c2 != "") cout << "2. " << c2 << endl;
    if (c3 != "") cout << "3. " << c3 << endl;
    cout << "--------------------------" << endl;
}