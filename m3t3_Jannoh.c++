/*
CSC 134
M3T3 - Craps and random numbers
AnnohJ
2/18/26
*/
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function Prototype - Declare what's coming
int roll (); // returns number from 1 to 6

int main () {
    // Seed the RNG
    srand(time(0));
    //Set up variables
    int roll1, roll2, total, point;
    string status; //win, lose, ...
   
   

    roll1 = roll();
    roll2 = roll();
    total = roll1 + roll2;

    // Print the dice roll
    cout << "Roll is: 🎲 " << roll1 << " + " << roll2 << " = " << total << endl;
    
    // Determine win or loss
    if (total == 7 || total == 11) { // || is or
        status = "win";
    }

    else if (total == 2 || total == 3 || total == 12) {
        status = "lose";
    }
    else {
        //point
        point = total; // save for later
        status = "point";
    }

    return 0;
}

// Function Definition - the whole thing
int roll () {
    // returns number from 1 to 6
    int num = (rand() % 6) + 1;
    return num;
}