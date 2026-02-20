/*
CSC 134
M2HW1 Part 3 - Gold
Josiah Annoh
2/19/2026
*/

#include <iomanip>
#include <iostream>

using namespace std;

int main () {
    int pizzas;
    int slices_per_pizza;
    int visitors;
    int total_slices;
    int leftovers;

    cout << "How many pizzas did you order? ";
    cin >> pizzas;

    cout << "How many slices per pizza? ";
    cin >> slices_per_pizza;

    cout << "How many visitors are coming? ";
    cin >> visitors;

    total_slices = pizzas * slices_per_pizza;
    leftovers = total_slices - (visitors * 3);

    cout << "Slices left over: " << leftovers << endl;

    return 0;
}