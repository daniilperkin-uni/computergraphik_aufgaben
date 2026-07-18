#ifndef pointlight_h
#define pointlight_h

#include "util.h"
#include "vec3.h"

/**
* @brief The Pointlight class.
*/
class Pointlight
{
public:
    /**
    * @brief Construct a point light with a pseudo random color and intensity.
    */
    Pointlight() :
        _position(0.),
        _color(1.),
        _intensity(2. + getRand() * 1.5) {}

    /**
    * @brief Construct a point light with a pseudo random color and intensity, located at a given position in the world.
    */
    Pointlight(Vec3d position) :
        _position(position),
        _color(getRand(), getRand(), getRand()),
        _intensity(2. + getRand() * 1.5) {}

    /**
    * @brief Construct a point light with given color and intensity, located at a given position in the world.
    */
    Pointlight(Vec3d position, Vec3d color, double intensity) :
        _position(position),
        _color(color),
        _intensity(intensity) {}

    /**
    * @brief Get the position of the point light.
    * @return The position of the light source.
    */
    Vec3d getPosition() const { return _position; }

    /**
    * @brief Get the color of the point light.
    * @return The color of the light source.
    */
    Vec3d getColor() const { return _color; }

    /**
    * @brief Get the intensity of the point light.
    * @return The intensity of the light source given in Lume.
    */
    double getIntensity() const { return _intensity; }

private:
    Vec3d _position;    //< position of the light source

    Vec3d _color;       //< color of the light source
    double _intensity;  //< intensity of the light source
};

#endif // !pointlight_h
