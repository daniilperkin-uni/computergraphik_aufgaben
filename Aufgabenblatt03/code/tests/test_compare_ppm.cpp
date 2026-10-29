// Regression test for the comparePPM() mismatch diagnostic (QA-009).
//
// 1000 differing pixels out of 360000 are 0.28 %, but the integer expression
// cnt * 100 / framebuffer.size() truncated that to "0% of the pixel values did
// not match" while the test was failing.
#include "util.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    constexpr unsigned int width = 600;
    constexpr unsigned int height = 600;
    const std::string path = "compare_ppm_percent.ppm";

    const std::vector<Vec3d> framebuffer(width * height, Vec3d(0.0, 0.0, 0.0));

    {
        std::ofstream file(path, std::ios::out | std::ios::binary);
        file << "P6\n" << width << " " << height << "\n255\n";

        for (unsigned int i = 0; i < width * height; ++i)
        {
            const char value = (i < 1000) ? static_cast<char>(255) : static_cast<char>(0);
            file << value << value << value;
        }
    }

    std::ostringstream captured;
    std::streambuf* previous = std::cout.rdbuf(captured.rdbuf());

    const bool match = comparePPM(path, "percent test", framebuffer);

    std::cout.rdbuf(previous);
    std::remove(path.c_str());

    if (match)
    {
        std::cerr << "FAIL: a mismatch of 1000 pixels was reported as a match\n";
        return 1;
    }

    const std::string message = captured.str();
    std::cout << "diagnostic: " << message;

    if (message.find("0.28%") == std::string::npos)
    {
        std::cerr << "FAIL: expected '0.28%' in the diagnostic\n";
        return 1;
    }

    std::cout << "comparePPM diagnostic passed\n";
    return 0;
}
