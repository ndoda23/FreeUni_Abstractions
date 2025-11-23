/*
 * File: Sierpinski.cpp
 * --------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Sierpinski problem
 * on Assignment #3.
 * [TODO: extend the documentation]
 */

#include <iostream>
#include "gwindow.h"
#include "simpio.h"
using namespace std;

void sierpinski(GWindow& window, int n, double x1, double y1, double x2, double y2, double x3, double y3);
double middleP(double, double);

int main() {
    // [TODO: fill with your code]
    GWindow window;
    int n = getInteger("Enter order: ");
    int length = getInteger("Enter length: ");
    
    // center of the bottom side.
    double bottomCenterX = window.getWidth() / 2;
    double bottomCenterY = window.getHeight()/2 + ((length*sqrt(3))/2)/3;
    double leftBottomX = bottomCenterX - length / 2;
    double leftBottomY = bottomCenterY;

    sierpinski(window,n+1,leftBottomX,leftBottomY, bottomCenterX , leftBottomY - length , bottomCenterX+length/2, leftBottomY);
    return 0;
}

// middle point.
double middleP(double num1, double num2) {
    return (num1 + num2)/2.0;
}
void sierpinski(GWindow& window, int n, double x1, double y1, double x2, double y2,double x3,double y3) {
   // if order is 0 that's it.
    if (n == 0) {
        return;
    }
    else {

        // it draws lines.
        window.drawLine(x1, y1, x2, y2);
        window.drawLine(x1, y1, x2, y2);
        window.drawLine(x2, y2, x3, y3);                                  
        window.drawLine(x3, y3, x1, y1);

        // it saves cordinates.
        double midX1 = middleP(x1, x2);
        double midY1 = middleP(y1, y2);
        double midX2 = middleP(x2, x3);
        double midY2 = middleP(y2, y3);
        double midX3 = middleP(x1, x3);
        double midY3 = middleP(y1, y3);


        sierpinski(window, n - 1,x1,y1,midX1,midY1,midX3,midY3);
        sierpinski(window, n - 1,midX1, midY1 ,x2,y2,midX2,midY2);
        sierpinski(window, n - 1, midX3,midY3,midX2,midY2,x3,y3);

    }
}