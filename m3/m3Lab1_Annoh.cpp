

#include <iostream>



using namespace std;

// List PROTOTYPES of all the choices up here
// rename these to something that makes sense!
void Fire();
void Lightning();
void Air();
void Water();

// main() goes here
int main() {

    string choice;
    // main() starts the game
    cout << "ELEMENTAL APOCALYPSE" << endl << endl;
    cout << "You're in an Apocalyptic World and need to chose an Elemental Power to save the world..." << endl;
    cout << "Choose Your Element: " << endl; // make these into real choices!
    cout << "1. Fire" << endl;
    cout << "2. Lightning" << endl;
    cout << "3. Air" << endl;
    cout << "4. Water" << endl;

    cout << "> "; // Give them a prompt to type
    cin >> choice;

    if ("Fire" == choice) {
        Fire();
    }

    else if ("Lightning" == choice) {
        Lightning();
    }
    else if ("Air" == choice) {
        Air();
    }
        else if ("Water" == choice) {
        Water();
    }
    else {
        cout << "I didn't understand that." << endl;
    }
    cout << "*GAME OVER*" << endl << endl;

    return 0; // end of game
}

// List FULL FUNCTIONS of all the choices at the bottom
void Fire() {
    cout << "You chose Fire." << endl;
    cout << "And then... you spawn right next to Giant Elemental Monster" << endl; // you finish it!
    cout << "1. Another Choice" << endl;
    cout << "2. Still another choice" << endl;
    int choice;
    cin >> choice;
}

void Lightning() {
    cout << "You chose Lightning." << endl;
    cout << "And then... you spawn right next to Giant Elemental Monster"; // you finish it!
}

void Air() {
    cout << "You chose Air." << endl;
    cout << "And then... you spawn right next to Giant Elemental Monster"; // you finish it!
}

void Water() {
    cout << "You chose Water." << endl;
    cout << "And then... you spawn right next to Giant Elemental Monster"; // you finish it!
}