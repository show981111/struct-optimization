#include <stdio.h>
#include <math.h>

// Define a struct for a circle with more than 5 fields
struct Circle
{
    double radius;
    double diameter;
    double area;
    double circumference;
    double x_center;
    double y_center;
    // Add more fields as needed
};

int main()
{
    // Create a circle with various properties
    struct Circle myCircle;
    myCircle.radius = 5.0;

    // Calculate additional properties
    myCircle.diameter = 2.0 * myCircle.radius;
    myCircle.area = 3.14 * pow(myCircle.radius, 2);
    myCircle.circumference = 2 * 3.14 * myCircle.radius;
    myCircle.x_center = 0.0;
    myCircle.y_center = 0.0;

    // Print the properties of the circle
    printf("Radius: %.2f\n", myCircle.radius);
    printf("Diameter: %.2f\n", myCircle.diameter);
    printf("Area: %.2f\n", myCircle.area);
    printf("Circumference: %.2f\n", myCircle.circumference);
    printf("Center Coordinates: (%.2f, %.2f)\n", myCircle.x_center, myCircle.y_center);

    // Add more print statements for additional fields

    return 0;
}
