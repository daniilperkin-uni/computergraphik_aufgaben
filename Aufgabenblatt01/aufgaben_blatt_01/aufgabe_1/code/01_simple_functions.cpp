#include <iostream>

using namespace std;

// Berechnet die Fläche eines Kreises
double circlearea(double r)
{
    const double PI = 3.14159265358979323846;
    return PI * r * r;
}

int main()
{
    double radius;

    cout << "Insert circle radius:" << std::endl;
    cin >> radius;

    if (radius < 0.0) {
        cerr << "Radius must not be negative" << std::endl;
        return 1;
    }

    double area = circlearea(radius);

    cout << "Area of circle with radius " << radius << " is: " << area << std::endl;

    return 0;
}
