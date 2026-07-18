#pragma once

#include "SceneObject.h"

#include <memory>
#include <string>
#include <vector>

namespace cg
{
    class Container : public SceneObject
    {
    public:
        /// <summary>
        /// Add object to the scene
        /// </summary>
        /// <param name="object">New object</param>
        void addObject(std::shared_ptr<SceneObject> object);

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
        /// Scene objects
        std::vector<std::shared_ptr<SceneObject>> objects;
    };
}
