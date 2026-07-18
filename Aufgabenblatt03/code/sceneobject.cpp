#include "sceneobject.h"

#include "util.h"
#include "vec3.h"

#include <cmath>
#include <utility>

/**
 * @brief SceneObject::SceneObject
 */
SceneObject::SceneObject() :
    _color(1.0, 1.0, 1.0)
{
}

/**
 * @brief SceneObject::SceneObject
     * @param color Color of the object, if it is not overwritten by the derived class.
 */
SceneObject::SceneObject(const Vec3d& color) :
    _color(color)
{
}

/**
 * @brief Plane::intersect
 */
bool Plane::intersect(const Ray &ray, double &t) const
{
    double denom = this->_normal.dot(ray.dir);

    // check if parallel
    if (!Vec3d::approxEq(denom, 0.))
    {
        const Vec3d origin2point = this->_point - ray.origin;

        t = origin2point.dot(this->_normal) / denom;

        return (t >= 0.);
    }

    return false;
}

/**
 * @brief Plane::getSurfaceColor
 * @param p_hit point where the surface was hit
 */
Vec3d Plane::getSurfaceColor(const Vec3d &p_hit) const
{
    // generate grey/white chess board pattern
    const double pi = std::acos(-1);

    const double freq = 0.125;
    const double s = cos(p_hit[0] * 2.0 * pi * freq) * cos(p_hit[2] * 2.0 * pi * freq);

    return Vec3d(0.2) + (s > 0) * Vec3d(0.4);
}


/**
 * @brief Sphere::intersect
 * @param ray Reference to the ray to be checked for intersection with objects.
 * @param t Reference to the intersection distance along the ray.
 */
/**
 * @brief Sphere::intersect
 * @param ray Reference to the ray to be checked for intersection with objects.
 * @param t Reference to the intersection distance along the ray.
 */
bool Sphere::intersect(const Ray &ray, double &t) const
{
    ///////////
    // TODO
    // Implement a ray-sphere intersection test.
    //
    // cf., lecture slides raytracing 37ff

    // Vektor vom Kugelmittelpunkt zum Strahlursprung
    Vec3d oc = ray.origin - this->_center;

    // Koeffizienten der quadratischen Gleichung at^2 + bt + c = 0
    double a = ray.dir.dot(ray.dir);
    double b = 2.0 * oc.dot(ray.dir);
    double c = oc.dot(oc) - this->_radius * this->_radius;

    // Diskriminante berechnen
    double discriminant = b * b - 4.0 * a * c;

    // Wenn die Diskriminante negativ ist, gibt es keinen Schnitt
    if (discriminant < 0.0) {
        return false;
    }

    // Die beiden Lösungen für t berechnen
    double sqrt_discriminant = sqrt(discriminant);
    double t1 = (-b - sqrt_discriminant) / (2.0 * a);
    double t2 = (-b + sqrt_discriminant) / (2.0 * a);

    // Finde den kleinsten positiven Schnittpunkt
    if (t1 > 0.0) {
        t = t1;
        return true;
    } else if (t2 > 0.0) {
        t = t2;
        return true;
    }

    // Beide Schnittpunkte liegen hinter dem Strahlursprung
    return false;

    // END TODO
    ///////////
}

/**
 * @brief Sphere::getSurfaceColor
 */
Vec3d Sphere::getSurfaceColor(const Vec3d &p_hit) const
{
    return this->_color;
}
