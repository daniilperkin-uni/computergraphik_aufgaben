#pragma once

#include "Math1.h"

#include <string>

namespace cg
{
    /// <summary>
    /// Transformable object
    /// </summary>
    class Object
    {
    public:
        /// <summary>
        /// Constructor (unit matrix)
        /// </summary>
        Object();

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="transformation">Initial transformation matrix</param>
        Object(const mat4& transformation);

        /// <summary>
        /// Return the name of the shape
        /// </summary>
        /// <returns>Shape name</returns>
        virtual std::string getShapeName() const = 0;

        /// <summary>
        /// Transform using a matrix
        /// </summary>
        /// <param name="transformation">Transformation matrix (multiplied)</param>
        void Transform(const mat4& transformation);

        /// <summary>
        /// Rotate object
        /// </summary>
        /// <param name="alpha">Degree rotating around x axis</param>
        /// <param name="beta">Degree rotating around y axis</param>
        /// <param name="gamma">Degree rotating around z axis</param>
        void Rotate(float alpha, float beta, float gamma);

        /// <summary>
        /// Translate object
        /// </summary>
        /// <param name="translation">Translation vector</param>
        void Translate(const vec3& translation);

        /// <summary>
        /// Scale object
        /// </summary>
        /// <param name="scale">Scale vector</param>
        void Scale(const vec3& scale);

        /// <summary>
        /// Get the model transform matrix
        /// </summary>
        /// <returns>Model matrix</returns>
        mat4 getTransformation() const;

        /// <summary>
        /// Access visibility property
        /// </summary>
        /// <returns>Visibility</returns>
        bool& accessVisibility();

        /// <summary>
        /// Is object visible?
        /// </summary>
        /// <returns>Visible?</returns>
        bool isVisible() const;

    protected:
        /// Model transform matrix
        mat4 transformation;

        /// Visibility
        bool visible;
    };
}
