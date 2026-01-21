// CSC 134
// M2T1 User Run Store
// name
// 1/14/26
/*
Were selling "food" -- you decide what is 
Tell the user how many you have,
how much each costs,
and what the total would be.

If we have time we'll make it more interactive
*/

#include <iostream>
// this is used for decimal point after price
#include <iomanip>
using namespace std;

int main () {
// Declare variables at the top, it's easier to read
string item_name;
int item_count;
int purchased; //How many the user wants
double price_each = 0.50;
double total_price;

//Set doubles to print with 2 decimal places
cout << setprecision(2) << fixed;

cout << "Welcome to our " << item_name << " store." << endl;
cout << "We have " << item_count << " " << item_name << endl;
cout << "They cost $" << price_each << " each." << endl;

//Find out how much the user wants to buy
cout << "How many would you like to buy? ";
cin >> purchased;

// Calculate the total
total_price = purchased * price_each;

cout << "Total price is $" << total_price << endl;

return 0;
}