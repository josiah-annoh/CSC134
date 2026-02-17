/*
m3t1
compare two triangles
*/
#include <iostream>
using namespace std;

int main() {
      // Program:measure two triangles
      //declare variables
      double length1, width1, area1;
      double length2, width2, area2;

    // get info from user
      cout << "Please enter the measurements for two rectanges." << endl << endl;
      cout << "Rectange 1" << endl;
      cout << "Length?";
      cin >> length1;
      cout << "Width";
      cin >> width1;
      cout << endl;
        cout << "Rectange 2" << endl;
      cout << "Length?";
      cin >> length2;
      cout << "Width";
      cin >> width2;

      //calculate the areas
      area1 = length1 * width1;
      area2 = length2 * width2;

      //print the output
      cout << "Rectangle 1 is area: " << area1 << endl;
      cout << "Rectangle 2 is area: " << area1 << endl;
    }