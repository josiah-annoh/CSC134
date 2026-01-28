/*
CSC 134
M2T2 - Receipt
Josiah Annoh
1/28/26
*/

#include <iostream>
#include <iomanip>

using namespace std;

int main() {

    // Variables 
    string meal_name = "Carolinia BBQ Burger";
    double meal_price = 5.99;
    int meal_count; // do they want 1 or 2 or ...
    // the receipt stuff
    double subtotal, tip, tax_amount, total; // all in $
    const double TAX_RATE = 0.07; // 7% is 7 / 100

    // Take the order
    cout << "Welcome to the restaurant " << endl;
    cout << "How many " << meal_name << " would you like? " << endl;
    cin >> meal_count;
    cout << "Preapring your order..." << endl << endl;

    // Calcualte the subtotal and total
    subtotal = meal_price * meal_count; // before taxes and tip
    tax_amount = subtotal * TAX_RATE;  // $ in taxes
    total = subtotal + tax_amount;

    // Print the receipt
    cout << setprecision(2) << fixed;
    cout << "------------------------------" << endl;
    cout << meal_name << " X " << meal_count << endl;
    cout << "SUBTOTAL: \t\t$"  << subtotal   << endl;
    cout << "Tax: \t\t\t$"     << tax_amount << endl;
    cout << "------------------------------" << endl;
    cout << "Total: \t\t\t$"   << total      << endl;
    return 0; 



}