/*
m7t2
annohj



*/


#include <iostream>
#include "Rectangle.h"
using namespace std;

//Function Prototypes
string setName(); 
void setName(string& name);

int main () {
    // Pointer Practice
    string name = "Bob";
    string * pName = &name;     // store the address in a pointer
//  setName(name);

    cout << "name   = " << name << endl;
//    cout << "pName  = " << pName << endl;
    cout << "*pName = " <<*pName << endl;

    // Part 2 - Rectangles
    Rectangle r1;
    double w, 1;
    cout << "Enter width and length seperated by a space";
    cin >> w >> 1;
    r1.setWidth(8);
    r1.setLength(10);
    cout << "Area is: " << r1.getArea() << endl;


}

//Full Fucntions

string setName () {
    string name;
    cout << "Enter name: ";
    cin >> name;
    return name;

}

void setName(string& name ) {
    cout << "Enter name: ";
    cin >> name;

}