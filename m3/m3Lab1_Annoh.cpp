

#include <iostream>

using namespace std;

// List PROTOTYPES of all the choices up here
// rename these to something that makes sense!
void Fire();
void Lightning();
void Air();
void Water();
void start_battle();

string elemental_name;


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
    
    cout << " *GAME OVER* " << endl << endl;

    return 0; // end of game
}

// List FULL FUNCTIONS of all the choices at the bottom
void Fire() {
    elemental_name = "Fire";
    cout << "You chose Fire." << endl;
    cout << "And then... you spawn in a random den" << endl; // you finish it!
    start_battle();
}


void Lightning() {
    elemental_name = "Lightning";
    cout << "You chose Lightning." << endl;
    cout << "And then... you spawn in a random den";
    start_battle();
}

void Air() {
    elemental_name = "Air";
    cout << "You chose Air." << endl;
    cout << "And then... you spawn in a random den" << endl;
    start_battle();
}

void Water() {
    elemental_name = "Water";
    cout << "You chose Water." << endl;
    cout << "And then... you spawn in a random den" << endl;
    start_battle();
}

void start_battle() {
    cout << "A Giant Elemental Zombie appears!" << endl;
    cout << "What do you do?" << endl;
    cout << "1. Shoot a " << elemental_name << " blast" << endl;
    cout << "2. Run away" << endl;

    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "You unleash your " << elemental_name << " power!" << endl;
        cout << "The zombie is defeated!" << endl;
    } else if (choice == 2) {
        cout << "You run away scared..." << endl;
        cout << "*GAME OVER*" << endl;
    }
}