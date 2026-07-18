#pragma once

#include "Lights/LightObject.h"
#include "Objects/SceneObject.h"

#include <memory>
#include <string>
#include <vector>

namespace cg
{
    /// <summary>
    /// Class holding objects for rendering a scene
    /// </summary>
    class Scene
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="name">Scene name</param>
        Scene(const std::string& name);

        /// <summary>
        /// Add object to the scene
        /// </summary>
        /// <param name="object">New object</param>
        void addObject(std::shared_ptr<SceneObject> object);

        /// <summary>
        /// Get objects
        /// </summary>
        /// <returns>Scene objects</returns>
        std::vector<std::shared_ptr<SceneObject>> getObjects();

        /// <summary>
        /// Add light source to the scene
        /// </summary>
        /// <param name="light">New light source</param>
        void addLight(std::shared_ptr<LightObject> light);

        /// <summary>
        /// Get light sources
        /// </summary>
        /// <returns>Light sources</returns>
        std::vector<std::shared_ptr<LightObject>> getLights();

        /// <summary>
        /// Get scene name
        /// </summary>
        /// <returns>Scene name</returns>
        const std::string& getName() const;

    private:
        /// Scene objects
        std::vector<std::shared_ptr<SceneObject>> objects;

        /// Light sources
        std::vector<std::shared_ptr<LightObject>> lights;

        /// Scene name
        std::string name;
    };
}
