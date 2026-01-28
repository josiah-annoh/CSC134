/*
CSC 134
M2LAB1 - Crates
norrisa
1/28/26
*/

#include <iostream>
#include <iomanip>

using namespace std;

int main() {

    // Part 1 - Set up Variables
    // Crate Variables
    double length, width, height, volume; // in feet
    // Price Variables
    const double COST_PER_CUBIC_FOOT = 0.23;
    const double CHARGE_PER_CUBIC_FOOT = 0.5;
    double cost, customer_price, profit; // in $

    // Part 2 - Input
    cout << "Crate Purchase Program" << endl;
    cout << "Enter the size of your crate \n";
    cout << "Length? ";
    cin >> length;
    cout << "Width? ";
    cin >> width;
    cout << "Height? ";
    cin >> height;


    // Part 3 - Calculation
    volume = length * width * height;

    // Part 4 - Output
    cout << setprecision(2) << fixed; // 2 decimal places
    cout << "\n---- CRATE INFO ----\n";
    cout << "Volume: " << volume << "cubic feet." << endl;


    return 0;
}