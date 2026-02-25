// CSC134
// m3lab2 - letter grade
// josiaha
// 2/25/26

#include <iostream>

using namespace std;

int main (){
    // Input - get the number grade
    double num_grade;
    string letter_grade;
    cout << "This program converts number grades (0-100) to letter grades." << endl;
    cout << "Enter grade: ";
    cin >> num_grade;

    // Calculate - find the letter grade
    if (num_grade >= 90) {
        letter_grade = "A";
    }

    else if (num_grade >= 80) {
        letter_grade = "B";

    }
    
    else if (num_grade >= 70) {
        letter_grade = "C";
    }

    else if (num_grade >= 60) {
        letter_grade = "D";
    }
    
    else {
        // less than 60
        letter_grade = "F";
    }

    // Output
    cout << "A grade of " << num_grade << " is letter grade: " << letter_grade << endl;


}
