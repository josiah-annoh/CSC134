/*
CSC 134
M8 Final Project - Elemental Apocalypse (Final Version)
m5lab1_Annoh.cpp - Entry point, utility functions
Jojo Annoh
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

#include "m5lab1_elemental.h"

// ============================================================
// ---- UTILITY: getPlayerChoice ----
// Fully validated: rejects non-integers, out-of-range values,
// and extra characters on the same line. Loops until valid.
// ============================================================
int getPlayerChoice(int maxChoice) {
    int choice;
    while (true) {
        cout << "Enter choice (1-" << maxChoice << "): ";
        if (cin >> choice) {
            // Consume any leftover characters on the line
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice >= 1 && choice <= maxChoice) {
                return choice;
            }
            cout << "!! Invalid — please enter a number between 1 and " << maxChoice << "." << endl;
        } else {
            // Non-integer input (e.g. a letter)
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "!! Invalid input — please enter a number." << endl;
        }
    }
}

// ============================================================
// ---- UTILITY: showChoices ----
// Displays three numbered options consistently.
// ============================================================
void showChoices(string c1, string c2, string c3) {
    cout << "1. " << c1 << endl;
    cout << "2. " << c2 << endl;
    cout << "3. " << c3 << endl;
}

// ============================================================
// ---- MAIN ----
// ============================================================
int main() {
    srand((unsigned int)time(nullptr));

    game_start();

    return 0;
}