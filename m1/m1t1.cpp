// csc 134 
// M1T1 (Module 1, tutorial 1)

//we'll explain this later
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    // Say hello
    cout << "Hello World!" << endl;
    cout << endl; // blank line
    cout << "Enter your first name: ";
    string name; // Declare a string variable
    cin >> name; // Getting input from user

    cout << "Hello " + name + " Welcome to C++! ";
    cout << endl; // blank line

    // Get numeric input and perform math
    int num1;

    cout << "Enter a whole number: ";
    cin >> num1; 

    cout << "Your Number is " + to_string(num1);
    cout << endl; // blank line

    //Another way to print integers in a string
    cout << "Your Number is " << num1 << endl;
    cout << endl; // blank line

    // Get the exponent from the user
    int exponent;
    cout << "Enter an exponent: ";
    cin >> exponent;

    // Create a new variable to hold the math answer
    // take num1 to the power of exponent
    int result = pow(num1,exponent);

    //Output
    cout << num1 << " raised to the power of " << exponent << " is " << result << endl;

    return 0; //exit with no errors
}