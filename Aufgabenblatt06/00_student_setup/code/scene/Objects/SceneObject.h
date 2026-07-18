#pragma once

#include "Math1.h"
#include "../Object.h"

namespace cg
{
    /// <summary>
    /// Scene object with ability to create a mesh of itself
    /// </summary>
    class SceneObject : public Object
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="color">Object color</param>
        SceneObject(const Color& color = white());

        /// <summary>
        /// Calculate and return a triangle mesh representing the object
        /// </summary>
        /// <returns>Triangle mesh</returns>
        virtual TriangleMesh calculateMesh() const = 0;

        /// <summary>
        /// Access object's color
        /// </summary>
        /// <returns>Object color</returns>
        Color& accessColor();

        /// <summary>
        /// Return the object's color
        /// </summary>
        /// <returns>Object color</returns>
        Color getColor() const;

    protected:
        /// Object color
        Color color;
    };
}
