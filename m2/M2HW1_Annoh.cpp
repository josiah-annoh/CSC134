/*
CSC 134
M2HW1 - Gold
Josiah Annoh
2/19/2026
*/

#include <iostream>
#include <iomanip>

using namespace std;

int main () {


    // Variables
    string name;
    double starting_balance;
    double deposit;
    double widthdrawl;
    double final_balance;
    int account_number = 200817;

    // Ask for name
    cout << "Enter your name: ";
    getline (cin, name);

    cout << "Enter your starting balance: ";
    cin >> starting_balance;

    cout << "Enter your deposit amount: ";
    cin >> deposit;
    
    cout << "Enter your widthdrawl amount: ";
    cin >> widthdrawl;

    final_balance = starting_balance + deposit - widthdrawl;

    cout << setprecision(2) << fixed;

    cout << "Name: " << name << endl;
    cout << "Account Number: " << account_number << endl;
    cout << "Final Balance: $" << final_balance << endl;

    return 0;
}
