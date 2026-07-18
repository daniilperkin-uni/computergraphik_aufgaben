#pragma once

#include <string>

#include "SceneObject.h"

namespace cg
{
    class TestTriangle : public SceneObject
    {
    public:
        /// <summary>
        /// Return the name of the shape
        /// </summary>
        /// <returns>Shape name</returns>
        virtual std::string getShapeName() const;

        /// <summary>
        /// Create the mesh for the triangle
        /// </summary>
        /// <returns></returns>
        cg::TriangleMesh calculateMesh() const;
    };
}
