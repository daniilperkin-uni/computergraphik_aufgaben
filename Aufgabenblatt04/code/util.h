#ifndef util_h
#define util_h

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "vec3.h"

static int state = { 42 };

/**
 * @brief Simple Ray class. A ray is defined with an origin and a direction.
 */
class Ray
{
public:
    Ray() : origin(Vec3d()), dir(Vec3d()), depth(0) {}
    ~Ray() {}

    Vec3d origin;   //< Orinin of the ray
    Vec3d dir;      //< Direction of the ray.
    int depth;      //< Recursive depth of the ray.
};


//////////////////////////////// Random number generation ////////////////////////////////
/**
 * @brief Own rand() implementation for cross platform deterministc 'randomness'.
 * @return pseudo random number
 */
static unsigned long cpRand()
{
    int const a = 1103515245;
    int const c = 12345;
    state = a * state + c;
    return (state >> 16) & 0x7FFF;
}

/**
 * @brief Generate a pseudorandom number in range [0,1] using the Mersenne Twister.
 * @return A random number in range [0,1] from a uniform distribution.
 */
static double getRand()
{
    std::mt19937 mtGen(cpRand());
    std::uniform_real_distribution<> distribNorm(0.0, 1.0);
    return distribNorm(mtGen);
}



/**
 * @brief Save an array of color values as a PPM image.
 * @param name The file name of the ppm file.
 * @param viewport The size of the viewport.
 * @param framebuffer Framebuffer containing the color values.
 */
static void saveAsPPM(const std::string name, const Vec3i viewport,
    const std::vector<Vec3d>& framebuffer)
{
    if (framebuffer.size() != size_t(viewport[0]) * viewport[1])
    {
        std::cerr << "Invalid framebuffer size, could not write out image." << std::endl;
        return;
    }

    std::ofstream os(name, std::ios::out | std::ios::binary);
    os << "P6\n" << viewport[0] << " " << viewport[1] << "\n255\n";
    for (size_t i = 0; i < framebuffer.size(); ++i)
    {
        Vec3d color = Vec3d::clamp(0., 1., framebuffer.at(i));
        char r = static_cast<char>(255 * color[0]);
        char g = static_cast<char>(255 * color[1]);
        char b = static_cast<char>(255 * color[2]);
        os << r << g << b;
    }
    os.close();
}

#endif // !util_h
