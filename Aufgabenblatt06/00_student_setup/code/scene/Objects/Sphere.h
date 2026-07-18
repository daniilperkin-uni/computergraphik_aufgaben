#pragma once

#include <string>

#include "SceneObject.h"

namespace cg
{
    /// <summary>
    /// Cube
    /// </summary>
    class Sphere : public SceneObject
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="radius">Sphere radius</param>
        /// <param name="tetaResolution">Resolution for spherical coordinate teta</param>
        /// <param name="phiResolution">Resolution for spherical coordinate phi</param>
        /// <param name="color">Color of the cube</param>
        Sphere(float radius = 1.0f, unsigned int tetaResolution = 25, unsigned int phiResolution = 50, const Color& color = oneVec4());

        /// <summary>
        /// Return the name of the shape
        /// </summary>
        /// <returns>Shape name</returns>
        virtual std::string getShapeName() const;

        /// <summary>
        /// Calculate and return a triangle mesh representing the object
        /// </summary>
        /// <returns>Triangle mesh</returns>
        virtual TriangleMesh calculateMesh() const;

    private:
        /// Radius
        float radius;

        /// Resolution for creating the mesh
        unsigned int tetaResolution;
        unsigned int phiResolution;
    };
}
