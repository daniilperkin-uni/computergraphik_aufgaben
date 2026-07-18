#pragma once

#include "Math1.h"

namespace cg
{
    class Camera
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="fov">Field of view</param>
        /// <param name="aspect">Aspect ratio</param>
        /// <param name="near">Distance to near plane</param>
        /// <param name="far">Distance to far plane</param>
        /// <param name="position">Camera position</param>
        /// <param name="lookat">Direction of camera</param>
        /// <param name="up">Camera up vector</param>
        Camera(float fov, float aspect, float near, float far, const Point3D& position, const Point3D& lookat, const vec3& up);

        /// <summary>
        /// Get projection and view transform matrices
        /// </summary>
        mat4 getProjection() const;
        mat4 getView() const;
        mat4 getViewProjection() const;

        /// <summary>
        /// Set or get camera parameters
        /// </summary>
        void setFov(float fov);
        void setAspect(float aspect);
        void setNear(float near);
        void setFar(float far);
        void setPosition(const Point3D& position);
        void setLookat(const Point3D& lookat);
        void setUp(const vec3& up);

        float getFov() const;
        float getAspect() const;
        float getNear() const;
        float getFar() const;
        const Point3D& getPosition() const;
        const Point3D& getLookat() const;
        const vec3& getUp() const;

        float& accessFov();
        float& accessAspect();
        float& accessNear();
        float& accessFar();
        Point3D& accessPosition();
        Point3D& accessLookat();
        vec3& accessUp();

    private:
        void calculateProjection();
        void calculateView();
        void calculateViewProjection();

        /// Camera parameters
        float fov;
        float aspect;
        float near;
        float far;

        Point3D position;
        Point3D lookat;
        vec3 up;

        /// Save matrices for fast access
        mat4 projection;
        mat4 view;
        mat4 viewProjection;
    };

    /// <summary>
    /// Function to create the default camera
    /// </summary>
    Camera defaultCamera();
}
