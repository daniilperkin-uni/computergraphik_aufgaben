// Regression test for Vec3::normalize() (QA-006).
//
// The template used to divide by length() in T, so normalizing a Vec3<int>
// zeroed the vector. normalize() now requires a floating-point type; this test
// pins the behaviour for the types that are actually used.
#include "vec3.h"

#include <cmath>
#include <iostream>

namespace
{
    int failures = 0;

    void check_close(const double actual, const double expected, const char* what)
    {
        if (std::abs(actual - expected) > 1e-6)
        {
            std::cerr << "FAIL: " << what << " is " << actual << " (expected " << expected << ")\n";
            ++failures;
        }
    }
}

int main()
{
    {
        Vec3d vector(3.0, 4.0, 0.0);
        vector.normalize();

        check_close(vector[0], 0.6, "Vec3d x");
        check_close(vector[1], 0.8, "Vec3d y");
        check_close(vector[2], 0.0, "Vec3d z");
        check_close(vector.length(), 1.0, "Vec3d length");
    }

    {
        Vec3f vector(0.0f, 3.0f, 4.0f);
        vector.normalize();

        check_close(vector[0], 0.0, "Vec3f x");
        check_close(vector[1], 0.6, "Vec3f y");
        check_close(vector[2], 0.8, "Vec3f z");
    }

    {
        Vec3d zero(0.0, 0.0, 0.0);
        zero.normalize();

        check_close(zero[0], 0.0, "zero vector x");
        check_close(zero[1], 0.0, "zero vector y");
        check_close(zero[2], 0.0, "zero vector z");
    }

    if (failures != 0)
    {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }

    std::cout << "All Vec3 checks passed\n";
    return 0;
}
