#include "sceneobject.h"

#include <cmath>
#include <tuple>
#include <utility>

#include "util.h"
#include "vec3.h"

static const double pi = std::acos(-1);

/**
 * @brief SceneObject::SceneObject
 */
SceneObject::SceneObject() :
    _color(1.0, 1.0, 1.0), _phongCoeff({ _color, _color, Vec3d(1.), 42. })
{
}

/**
 * @brief SceneObject::SceneObject
     * @param color Color of the object, if it is not overwritten by the derived class.
 */
SceneObject::SceneObject(const Vec3d& color) :
    _color(color), _phongCoeff({ _color, _color, Vec3d(1.), 42. })
{
}

/**
 * @brief Plane::intersect
 */
bool Plane::intersect(const Ray& ray, double& t) const
{
    double denom = this->_normal.dot(ray.dir);
    if (denom < -1.e-6)   // avoid zero div
    {
        //Vec3d origin2point = this->_point - ray.origin;
        Vec3d origin2point = ray.origin - this->_point;
        t = origin2point.dot(this->_normal) / -denom;
        return (t >= 0);
    }
    return false;
}

Vec3d Plane::getSurfaceNormal(const Vec3d& p_hit) const
{
    return this->_normal;
}

/**
 * @brief Plane::getSurfaceColor
 */
Vec3d Plane::getSurfaceColor(const Vec3d& p_hit) const
{
    // generate grey chess board pattern
    const double freq = 0.125;
    double s = cos(p_hit[0] * 2. * pi * freq) * cos(p_hit[2] * 2. * pi * freq);
    return Vec3d(0.2) + (s > 0) * Vec3d(0.4);
}

PhongCoefficients Plane::getPhongCoefficients(const Vec3d& p_hit) const
{
    // generate grey chess board pattern
    const double freq = 0.125;
    double s = cos(p_hit[0] * 2. * pi * freq) * cos(p_hit[2] * 2. * pi * freq);
    Vec3d color = Vec3d(0.2) + (s > 0) * Vec3d(0.4);

    PhongCoefficients phongCoeff;
    std::get<0>(phongCoeff) = color;
    std::get<1>(phongCoeff) = color;
    std::get<2>(phongCoeff) = Vec3d(1.);
    std::get<3>(phongCoeff) = 42.;

    return phongCoeff;
}


/**
 * @brief Sphere::intersect
 */
bool Sphere::intersect(const Ray& ray, double& t) const
{

    // Implement a ray-sphere intersection test.
    // cf. lecture slides 46ff
    double t0 = -1;
    double t1 = -1;

#if 0
    // geometric solution
    Vec3d L = this->_center - ray.origin;
    double tca = L.dot(ray.dir);
    if (tca < 0)
        return false;
    double d2 = L.dot(L) - tca * tca;
    if (d2 > this->_radius * this->_radius)
        return false;
    double thc = sqrt(this->_radius * this->_radius - d2);
    t0 = tca - thc;
    t1 = tca + thc;
#else
    // analytic solution
    Vec3d L = ray.origin - this->_center;
    double a = ray.dir.dot(ray.dir);
    double b = 2.f * ray.dir.dot(L);
    double c = L.dot(L) - this->_radius * this->_radius;
    // solve quadratic function
    double discr = b * b - 4.f * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
    {
        t0 = -0.5f * b / a;
        t1 = t0;
    }
    else
    {
        double q = (b > 0) ? -0.5f * (b + sqrt(discr)) : -0.5f * (b - sqrt(discr));
        t0 = q / a;
        t1 = c / q;
    }
#endif

    if (t0 > t1)
        std::swap(t0, t1);

    if (t0 < 0)
    {
        t0 = t1; // use t1 if t0 is negative
        if (t0 < 0)
            return false; // both negative
    }

    t = t0;

    return true;
}

Vec3d Sphere::getSurfaceNormal(const Vec3d& p_hit) const
{
    return (p_hit - this->_center).normalize();
}

/**
 * @brief Sphere::getSurfaceColor
 */
Vec3d Sphere::getSurfaceColor(const Vec3d& p_hit) const
{
    return this->_color;
}

PhongCoefficients Sphere::getPhongCoefficients(const Vec3d& p_hit) const
{
    return this->_phongCoeff;
}
