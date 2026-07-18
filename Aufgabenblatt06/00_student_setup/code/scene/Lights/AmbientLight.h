#pragma once

#include <string>

#include "LightObject.h"

namespace cg
{
    /// <summary>
    /// Ambient light
    /// </summary>
    class AmbientLight : public LightObject
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="color">Color</param>
        /// <param name="intensity">Intensity of the light source</param>
        AmbientLight(const Color& color, float intensity = 0.1f);

        /// <summary>
        /// Return the name of the shape
        /// </summary>
        /// <returns>Shape name</returns>
        virtual std::string getShapeName() const;

        /// <summary>
        /// Return the light's color
        /// </summary>
        /// <returns>Color</returns>
        ColorInfo getColor(const Point3D&) const;
    };
}
