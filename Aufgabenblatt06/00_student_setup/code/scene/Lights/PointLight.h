#pragma once

#include <string>

#include "LightObject.h"

namespace cg
{
    /// <summary>
    /// Ambient light
    /// </summary>
    class PointLight : public LightObject
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="color">Color</param>
        /// <param name="intensity">Intensity of the light source</param>
        PointLight(const Color& color, float intensity = 1.0f);

        /// <summary>
        /// Return the name of the shape
        /// </summary>
        /// <returns>Shape name</returns>
        virtual std::string getShapeName() const;

        /// <summary>
        /// Return the light's color
        /// </summary>
        /// <returns>Color</returns>
        /// <param name="obj">Object position</param>
        ColorInfo getColor(const Point3D& obj) const;
    };
}
