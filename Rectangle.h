// Rectangle.h

#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
    private:
    double width;
    double length;

    public: 
    double getWidth() const {
        return width;
    }
    double getLength() const {
        return length;
    }
    double getArea() const {
        // always calclated fresh
        double area = length * width;
        return area;
    }
    void setWidth(double w) {
        width = w;
    }
    void setLength(double l) {
        length = l;
    }
    void drawRectangle() {
        string pixel = "*"; // use whatever
        for (int i=0; i< length; i)
    }





};

#endif // RECTANGLE_H