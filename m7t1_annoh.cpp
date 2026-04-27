/*
M7T1 - Restaurant Ratings
First OOP code
annohj
4/27/26
*/

#include <iostream>
using namespace std;

// Restaurant class -- could go in its own Restaurant.h
class Restaurant {
    private:
     string name;       // name of restaurant
     double rating;     // 1.0 - 5.0, including half stars
    public:
     Restaurant(string n, double r); // constructor
     void setName(string n);
     void setRating(double r);
     string getName() const;
     double getRating() const;

};

// Restaurant functions -- could go in Restaurant.h or .cpp
Restaurant::Restaurant(string n, double r) {
    // constructor
    name = n;
    rating = r;
}

void Restaurant::setName(string n) {
    name = n;
}

void Restaurant::setRating(double r) {
    rating = r;
}

string Restaurant::getName() const {
    return name;
}

double Restaurant::getRating() const {
    return rating;
}

// main
int main() {
    // Quick Test
    string name;
    double rating;

    cout << "Restaurant Review" << endl;
    cout << "Enter retaurant name: ";
    getline(cin, name);
    cout << "Enter restaurant rating: ";
    cin >> rating;

    Restaurant rest1 = Restaurant(name, rating);
    cout << rest1.getName() << endl;
    cout << rest1.getRating() << endl;
}