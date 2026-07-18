#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

cg::Camera::Camera(const float fov, const float aspect, const float near, const float far, const Point3D& position, const Point3D& lookat, const vec3& up)
    : fov(fov), aspect(aspect), near(near), far(far), position(position), lookat(lookat), up(up)
{
    calculateProjection();
    calculateView();
    calculateViewProjection();
}

void cg::Camera::calculateProjection()
{
    this->projection = glm::perspective(glm::template pi<float>() * this->fov / 180.0f, this->aspect, this->near, this->far);
}

void cg::Camera::calculateView()
{
    this->view = glm::lookAt(this->position, this->lookat, this->up);
}

void cg::Camera::calculateViewProjection()
{
    this->viewProjection = this->projection * this->view;
}

cg::mat4 cg::Camera::getProjection() const
{
    return this->projection;
}

cg::mat4 cg::Camera::getView() const
{
    return this->view;
}

cg::mat4 cg::Camera::getViewProjection() const
{
    return this->viewProjection;
}

void cg::Camera::setFov(const float fov)
{
    this->fov = fov;

    calculateProjection();
    calculateViewProjection();
}

void cg::Camera::setAspect(const float aspect)
{
    this->aspect = aspect;

    calculateProjection();
    calculateViewProjection();
}

void cg::Camera::setNear(const float near)
{
    this->near = near;

    calculateProjection();
    calculateViewProjection();
}

void cg::Camera::setFar(const float far)
{
    this->far = far;

    calculateProjection();
    calculateViewProjection();
}

void cg::Camera::setPosition(const Point3D& position)
{
    this->position = position;

    calculateView();
    calculateViewProjection();
}

void cg::Camera::setLookat(const Point3D& lookat)
{
    this->lookat = lookat;

    calculateView();
    calculateViewProjection();
}

void cg::Camera::setUp(const vec3& up)
{
    this->up = up;

    calculateView();
    calculateViewProjection();
}

float cg::Camera::getFov() const
{
    return this->fov;
}

float cg::Camera::getAspect() const
{
    return this->aspect;
}

float cg::Camera::getNear() const
{
    return this->near;
}

float cg::Camera::getFar() const
{
    return this->far;
}

const cg::Point3D& cg::Camera::getPosition() const
{
    return this->position;
}

const cg::Point3D& cg::Camera::getLookat() const
{
    return this->lookat;
}

const cg::vec3& cg::Camera::getUp() const
{
    return this->up;
}

float& cg::Camera::accessFov()
{
    return this->fov;
}

float& cg::Camera::accessAspect()
{
    return this->aspect;
}

float& cg::Camera::accessNear()
{
    return this->near;
}

float& cg::Camera::accessFar()
{
    return this->far;
}

cg::Point3D& cg::Camera::accessPosition()
{
    return this->position;
}

cg::Point3D& cg::Camera::accessLookat()
{
    return this->lookat;
}

cg::vec3& cg::Camera::accessUp()
{
    return this->up;
}

cg::Camera cg::defaultCamera()
{
    return Camera(60.0f, 1.0f, 0.1f, 1000.0f, zeroVec3(), Point3D(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));
}
