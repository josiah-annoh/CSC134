// CSC 134 - M5HW1
// All 6 questions implemented as functions with a main menu (Gold tier)

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// ─────────────────────────────────────────────
// Question 1: Average Rainfall
// ─────────────────────────────────────────────
void question1() {
    string month1, month2, month3;
    double rain1, rain2, rain3;

    cout << "Enter month: ";
    cin >> month1;
    cout << "Enter rainfall for " << month1 << ": ";
    cin >> rain1;

    cout << "Enter month: ";
    cin >> month2;
    cout << "Enter rainfall for " << month2 << ": ";
    cin >> rain2;

    cout << "Enter month: ";
    cin >> month3;
    cout << "Enter rainfall for " << month3 << ": ";
    cin >> rain3;

    double avg = (rain1 + rain2 + rain3) / 3.0;

    cout << fixed << setprecision(2);
    cout << "The average rainfall for " << month1 << ", " << month2
         << ", and " << month3 << " is " << avg << " inches." << endl;
}

// ─────────────────────────────────────────────
// Question 2: Volume of a Hyperrectangle (Block)
// ─────────────────────────────────────────────
void question2() {
    double width, length, height;

    // Width
    do {
        cout << "Enter width: ";
        cin >> width;
        if (width <= 0)
            cout << "Width must be greater than zero." << endl;
    } while (width <= 0);

    // Length
    do {
        cout << "Enter length: ";
        cin >> length;
        if (length <= 0)
            cout << "Length must be greater than zero." << endl;
    } while (length <= 0);

    // Height
    do {
        cout << "Enter height: ";
        cin >> height;
        if (height <= 0)
            cout << "Height must be greater than zero." << endl;
    } while (height <= 0);

    double volume = width * length * height;
    cout << fixed << setprecision(2);
    cout << "The volume of the block is " << volume << endl;
}

// ─────────────────────────────────────────────
// Question 3: Roman Numerals (1–10)
// ─────────────────────────────────────────────
void question3() {
    int number;

    do {
        cout << "Enter a number (1 - 10): ";
        cin >> number;
        if (number < 1 || number > 10)
            cout << "Please enter a number between 1 and 10." << endl;
    } while (number < 1 || number > 10);

    string roman;
    switch (number) {
        case 1:  roman = "I";    break;
        case 2:  roman = "II";   break;
        case 3:  roman = "III";  break;
        case 4:  roman = "IV";   break;
        case 5:  roman = "V";    break;
        case 6:  roman = "VI";   break;
        case 7:  roman = "VII";  break;
        case 8:  roman = "VIII"; break;
        case 9:  roman = "IX";   break;
        case 10: roman = "X";    break;
    }

    cout << "The Roman numeral version of " << number << " is " << roman << "." << endl;
}

// ─────────────────────────────────────────────
// Question 4: Geometry Calculator
// ─────────────────────────────────────────────
void question4() {
    const double PI = 3.14159;
    int choice;

    cout << "\nGeometry Calculator" << endl;
    cout << "1. Calculate the Area of a Circle" << endl;
    cout << "2. Calculate the Area of a Rectangle" << endl;
    cout << "3. Calculate the Area of a Triangle" << endl;
    cout << "4. Quit" << endl;
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    if (choice < 1 || choice > 4) {
        cout << "The valid choices are 1 through 4. Run the program again and select one of those." << endl;
        return;
    }

    if (choice == 4) {
        cout << "Returning to main menu." << endl;
        return;
    }

    if (choice == 1) {
        double radius;
        cout << "Enter the circle's radius: ";
        cin >> radius;
        if (radius < 0) {
            cout << "The radius cannot be less than zero." << endl;
            return;
        }
        double area = PI * radius * radius;
        cout << "The area is " << area << endl;
    }
    else if (choice == 2) {
        double length, width;
        cout << "Enter the rectangle's length: ";
        cin >> length;
        cout << "Enter the rectangle's width: ";
        cin >> width;
        if (length < 0 || width < 0) {
            cout << "Only enter positive values for length and width." << endl;
            return;
        }
        double area = length * width;
        cout << "The area is " << area << endl;
    }
    else if (choice == 3) {
        double base, height;
        cout << "Enter the triangle's base: ";
        cin >> base;
        cout << "Enter the triangle's height: ";
        cin >> height;
        if (base < 0 || height < 0) {
            cout << "Only enter positive values for base and height." << endl;
            return;
        }
        double area = base * height * 0.5;
        cout << "The area is " << area << endl;
    }
}

// ─────────────────────────────────────────────
// Question 5: Distance Traveled
// ─────────────────────────────────────────────
void question5() {
    double speed;
    int hours;

    do {
        cout << "What is the speed of the vehicle in mph? ";
        cin >> speed;
        if (speed < 0)
            cout << "Speed cannot be negative." << endl;
    } while (speed < 0);

    do {
        cout << "How many hours has it traveled? ";
        cin >> hours;
        if (hours < 1)
            cout << "Time traveled must be at least 1 hour." << endl;
    } while (hours < 1);

    cout << "\nHour    Distance Traveled" << endl;
    cout << "--------------------------------" << endl;
    for (int h = 1; h <= hours; h++) {
        cout << setw(4) << h << setw(12) << (speed * h) << endl;
    }
}

// ─────────────────────────────────────────────
// Question 6: Main Menu
// ─────────────────────────────────────────────
int main() {
    int choice;

    do {
        cout << "\n===== Main Menu =====" << endl;
        cout << "1. Average Rainfall (Q1)" << endl;
        cout << "2. Volume of a Block (Q2)" << endl;
        cout << "3. Roman Numerals (Q3)" << endl;
        cout << "4. Geometry Calculator (Q4)" << endl;
        cout << "5. Distance Traveled (Q5)" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1: question1(); break;
            case 2: question2(); break;
            case 3: question3(); break;
            case 4: question4(); break;
            case 5: question5(); break;
            case 6: cout << "Goodbye!" << endl; break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 6." << endl;
        }
    } while (choice != 6);

    return 0;
}