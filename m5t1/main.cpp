// M5t1 - More Functions
// annohj
// 3/18/26
// Show off different function types




#include <iostream>

using namespace std;


// Function Declarations
void say_hi();
int show_answer();
double square_a_number (double number);

// Main () goes here
int main()
{
    say_hi ();
    cout << "the answer is: " << show_answer() << endl;
    cout << "Enter a number; ";
    double number;
    cin >> number;
    double answer = square_a_number(number);
    cout << number << " squared is " << answer << endl;
    return 0;
}

// Function definitions

void say_hi () {
    cout << "Hello world!" << endl;
    return; //optional, assumed
}

int show_answer () {
    int answer = 42;
    return 42; // return is required because it's not void
}

double square_a_number(double number) {
    double square = number * number;
    return square;
}
