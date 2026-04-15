// CSC 134
// M3HW1 - Gold
// Josiah Annoh
// 4/14/26

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>   // For random number seeding
#include <cstdlib> // For rand() and srand()

using namespace std;

int main() {
    // --- Question 1: Chat Bot ---
    cout << "--- Question 1 ---" << endl;
    string choice;
    cout << "Hello, I'm a C++ program!" << endl;
    cout << "Do you like me? Please type yes or no: ";
    cin >> choice;

    if (choice == "yes") {
        cout << "That's great! I'm sure we'll get along." << endl;
    } else if (choice == "no") {
        cout << "Well, maybe you'll learn to like me later." << endl;
    } else {
        cout << "If you're not sure... that's OK." << endl;
    }
    cout << endl;

    // --- Question 2: Receipt Calculator ---
    cout << "--- Question 2 ---" << endl;
    double mealPrice;
    int orderType;
    double taxRate = 0.07; // Assuming 7% tax
    double tip = 0.0;

    cout << "Please enter the price of the meal: ";
    cin >> mealPrice;
    cout << "Enter 1 if the order is dine in, 2 if it is to go: ";
    cin >> orderType;

    if (orderType == 1) {
        tip = mealPrice * 0.15;
    }

    double tax = mealPrice * taxRate;
    double total = mealPrice + tax + tip;

    cout << fixed << setprecision(2);
    cout << "\n--- Receipt ---" << endl;
    cout << "Meal Price: $" << mealPrice << endl;
    cout << "Tax:        $" << tax << endl;
    if (orderType == 1) {
        cout << "Tip (15%):  $" << tip << endl;
    }
    cout << "----------------" << endl;
    cout << "Total Due:  $" << total << endl;
    cout << endl;

    // --- Question 3: Choose Your Own Adventure ---
    cout << "--- Question 3 ---" << endl;
    int move;
    cout << "You are standing in front of a dark cave. Do you:" << endl;
    cout << "1. Enter the cave" << endl;
    cout << "2. Walk away" << endl;
    cin >> move;

    if (move == 1) {
        cout << "You see a treasure chest and a sleeping dragon. Do you:" << endl;
        cout << "1. Try to steal the treasure" << endl;
        cout << "2. Sneak past quietly" << endl;
        cin >> move;

        if (move == 2) {
            cout << "You successfully found a secret exit! You Win!" << endl;
        } else {
            cout << "The dragon woke up... Game Over." << endl;
        }
    } else {
        cout << "You went home safely, but you'll always wonder what was inside. Defeat." << endl;
    }
    cout << endl;

    // --- Question 4: Math Practice ---
    cout << "--- Question 4 ---" << endl;
    srand(time(0)); // Seeds the random number generator
    int num1 = rand() % 10; // Random number 0-9
    int num2 = rand() % 10;
    int userAnswer;

    cout << "What is " << num1 << " plus " << num2 << "?" << endl;
    cin >> userAnswer;

    if (userAnswer == (num1 + num2)) {
        cout << "Correct!" << endl;
    } else {
        cout << "Incorrect." << endl;
    }

    return 0;
}