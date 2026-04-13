#ifndef M5T_H_INCLUDED
#define M5T_H_INCLUDED
using namespace std;

// Function definitions (FULL CODE)

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

#endif // M5T_H_INCLUDED
