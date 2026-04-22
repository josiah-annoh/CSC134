/*
M6LAB1 - Slime Roulette
annohj
4/15/26
*/
#include <iostream>
#include <vector>
#include <algorithm>   // std::shuffle
#include <random>      // std::mt19937

using namespace std;

// Function prototypes
void loadMagazine(vector<char>& mag,
                  int slime, int water);
void displayMagazine(const vector<char>& mag);
char fireShot(vector<char>& mag);

// Main()
int main() {
    vector<char> magazine;   // no size yet — grows as needed

    // Now let's test it
    int slime=3;
    int water=2;
    loadMagazine(magazine, slime, water);
    // How full is it? Just ask.
    //cout << "Mag size: " << magazine.size();
    // Show what's loaded
    displayMagazine(magazine); 

    // Fire a shot
    if (!magazine.empty()) {
        char s = fireShot(magazine);
        cout << "You fired: " << s << endl;
    }
    // list again after the shot
    displayMagazine(magazine);
    // What else do we need?
    // Taking turns, dealer and player alternate
    // Hit points (-1 HP from slime)
    // Maybe some items
    // Each round changes what's loaded?
    // and more

}

// Full Functions go here
void loadMagazine(vector<char>& mag,
                  int slime, int water) {
    mag.clear();    // fresh start (also handles reload!)

    for (int i = 0; i < slime; i++) mag.push_back('S');
    for (int i = 0; i < water; i++) mag.push_back('W');

    // One line. Properly random. Done.
    random_device rd;
    mt19937 gen(rd());
    shuffle(mag.begin(), mag.end(), gen);

    // Example of how to use fireShot()
    /*
    // Calling code can ask before firing:
    if (!mag.empty()) {
        char s = fireShot(mag);
        // ...
    }
    */
}

void displayMagazine(const vector<char>& mag) {
    int s = 0, w = 0;

    // "for each shell in mag" — no indices, no bounds
    for (char shell : mag) {
        if (shell == 'S') s++;
        else               w++;
    }

    cout << "Magazine: " << mag.size()
         << " remaining ("
         << s << " slime, "
         << w << " water)\n";
}

char fireShot(vector<char>& mag) {
    char shell = mag.back();   // peek the next shell
    mag.pop_back();             // remove it from the magazine
    return shell;
}
