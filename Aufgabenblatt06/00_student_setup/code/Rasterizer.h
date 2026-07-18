#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <vector>

#include "scene/Camera.h"
#include "image/Image.h"
#include "scene/Scene.h"
#include "scene/Objects/SceneObject.h"

namespace cg
{
    /// <summary>
    /// Rasterizer
    /// </summary>
    class Rasterizer
    {
    public:
        enum rasterization_mode
        {
            POINTS, WIREFRAME, FILLED
        };

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="camera">Camera</param>
        /// <param name="scenes">Scenes</param>
        /// <param name="mode">Rasterization mode</param>
        /// <param name="width">Image width</param>
        /// <param name="height">Image height</param>
        Rasterizer(Camera camera, const std::vector<Scene>& scenes, rasterization_mode mode, unsigned int width, unsigned int height);

        /// <summary>
        /// Draw scene
        /// </summary>
        /// <param name="rotate">Animated rotation?</param>
        void draw(bool rotate);

        /// <summary>
        /// Access camera
        /// </summary>
        /// <returns>Camera</returns>
        Camera& accessCamera();

        /// <summary>
        /// Access scene
        /// </summary>
        /// <returns>Scene</returns>
        Scene& accessScene();

        /// <summary>
        /// Get scenes
        /// </summary>
        /// <returns>Scenes</returns>
        const std::vector<Scene>& getScenes() const;

        /// <summary>
        /// Access active scene index
        /// </summary>
        /// <returns>Scene index</returns>
        int& accessActiveScene();

        /// <summary>
        /// Get number of available scenes
        /// </summary>
        /// <returns>Number of scenes</returns>
        int getNumScenes() const;

        /// <summary>
        /// Access rasterization mode
        /// </summary>
        /// <returns>Rasterization mode</returns>
        rasterization_mode& accessMode();

        /// <summary>
        /// Access image
        /// </summary>
        /// <returns>Image</returns>
        cg::image<cg::color_space_t::RGBA>& accessImage();

        /// <summary>
        /// Access rotation speed
        /// </summary>
        /// <returns>Rotation speed</returns>
        float& accessRotationSpeed();

        /// <summary>
        /// Access rotation axis
        /// </summary>
        /// <returns>Rotation axis</returns>
        vec3& accessRotationAxis();

    private:
        /// <summary>
        /// Draw object
        /// </summary>
        /// <param name="object">Object</param>
        /// <param name="transformation">Additional transformation for objects (excluding lights)</param>
        void drawObject(const std::shared_ptr<SceneObject> object, const mat4& transformation);

        /// <summary>
        /// Draw triangle
        /// </summary>
        /// <param name="triangle">Triangle</param>
        void drawTriangle(const Triangle& triangle);

        /// <summary>
        /// Draw triangle points
        /// </summary>
        /// <param name="triangle">Triangle</param>
        void rasterizePoints(const Triangle& triangle);

        /// <summary>
        /// Draw triangle edges
        /// </summary>
        /// <param name="triangle">Triangle</param>
        void drawWireframe(const Triangle& triangle);

        /// <summary>
        /// Draw filled triangle
        /// </summary>
        /// <param name="triangle">Triangle</param>
        void rasterizeFilled(const Triangle& triangle);

        /// <summary>
        /// Draw line
        /// </summary>
        /// <param name="point_start">Starting point of the line</param>
        /// <param name="point_end">End point of the line</param>
        void rasterizeLine(const Triangle::Point& point_start, const Triangle::Point& point_end);

        /// <summary>
        /// Draw single pixel
        /// </summary>
        /// <param name="point">Point</param>
        /// <param name="color">Color</param>
        void setPixel(const Point3D& point, Color color);

        /// Camera
        Camera camera;

        /// Scene
        std::vector<Scene> scenes;
        int activeScene = 0;

        /// Rasterization mode
        rasterization_mode mode;

        /// Rasterized image
        cg::image<cg::color_space_t::RGBA> image;

        /// z-Buffer
        cg::image<cg::color_space_t::Gray> zBuffer;

        /// Last rotation time and rotation speed
        std::chrono::milliseconds lastRotation;
        float rotationSpeed;
        vec3 rotationAxis;
    };
}