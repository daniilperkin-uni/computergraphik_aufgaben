#pragma once

#include <string>

#include "SceneObject.h"

namespace cg
{
    /// <summary>
    /// Cube
    /// </summary>
    class Cube : public SceneObject
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="edgeLength">Length of the edges</param>
        /// <param name="color">Color of the cube</param>
        Cube(float edgeLength = 1.0f, const Color& color = oneVec4());

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
        /// Edge length
        float edgeLength;
    };
}
