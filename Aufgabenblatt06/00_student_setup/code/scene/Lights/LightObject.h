#pragma once

#include "Math1.h"
#include "../Object.h"
#include "glm/vec3.hpp"

namespace cg
{
    /// <summary>
    /// Color information from the light source
    /// </summary>
    struct ColorInfo
    {
        Color color;
        float intensity;
        bool ambient;
        glm::vec3 ray;
    };

    /// <summary>
    /// Light object
    /// </summary>
    class LightObject : public Object
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="color">Color</param>
        /// <param name="intensity">Intensity of the light source</param>
        /// <param name="ambient">Is it an ambient light source?</param>
        LightObject(const Color& color, float intensity = 1.0f, bool ambient = false);

        /// <summary>
        /// Calculate and return the light's color
        /// </summary>
        /// <param name="obj">Position of the object to shine upon</param>
        /// <returns>Color</returns>
        virtual ColorInfo getColor(const Point3D& obj) const = 0;

        /// <summary>
        /// Access color
        /// </summary>
        /// <returns>Color</returns>
        Color& accessColor();

        /// <summary>
        /// Access light intensity
        /// </summary>
        /// <returns>Intensity</returns>
        float& accessIntensity();

    protected:
        /// Color and intensity
        Color color;
        float intensity;
        bool ambient;
    };
}
