/*
mbt1 - XP Tracker
annohj
4/13/26
gather XP per floor, then do stats.


*/

#include <iostream>
using namespace std;

// chart function
void barChart (int xp[], int floors);

int main () {
    // Set up variables
    const int SIZE = 5; // can only be changed at compile time
    int xp[SIZE];       // fixed size
    double total = 0.0;
    double average;
    int max = 0;
    // Input
    cout << "Dungeon XP Tracker" << endl;
    for (int i=0; i<SIZE; i++) {
        // get XP for floors 1-5
        cout << "Floor " << i+1 << " XP: ";
        cin >> xp[i];
    }


    // Calculation
    for (int i=0; i<SIZE; i++) {
        // running total
        total += xp[i];
        if (xp[i] > max) {
            max = xp[i]; // if current is larger than max, its the new max
        }
    }
    average = total / SIZE;
    //Output
    cout << "Over " << SIZE << " Floors" << endl;
    cout << "Total XP: " << total << endl;
    cout << "Avg XP: " << average << endl;
    cout << "Best XP: " << max << endl;

    // run bar chart
    barChart(xp, SIZE);
    
    //end program
    return 0;

}
void barChart(int xp[], int floors) {
    const int SCALE = 2; // 1 bar = 10 XP
    for (int i = 0; i < floors; i++) {
        cout << "F" << (i+1) << " | ";
        for (int b = 0; b < xp[i]/SCALE; b++)
            cout << "😎"; // or use your favorite emoji
        cout << " " << xp [i] << "\n";
    }
}