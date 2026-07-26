#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>

#include "pointlight.h"
#include "scene.h"
#include "sceneobject.h"
#include "util.h"
#include "vec3.h"

const static int WIDTH = 600;
const static int HEIGHT = 600;
const static int MAX_DEPTH = 5;

//////////
// TODO 2:
// Compute Phong lighting
//
// Computes the per-light diffuse and specular contributions only.
// The ambient term (k_a) is NOT included here: it is added exactly once in
// computeDirectLighting() to avoid the ambient double-counting that occurs when
// every light re-adds k_a.
Vec3d computePhongLighting(
    Vec3d const& view_direction,            //< direction from surface point to camera origin
    Vec3d const& surface_normal,            //< normal vector at surface point
    Vec3d const& light_direction,           //< direction from surface point to light source

    PhongCoefficients const& phong_coeff,   //< phong coefficient k_a,k_d,k_s and n
    Vec3d const& light_color,               //< color of the light source
    double light_intensity)                 //< intensity of the light source
{
    // Diffuse reflection (Lambert).
    const double diff_dot = std::max(0.0, surface_normal.dot(light_direction));
    const Vec3d diffuse = std::get<1>(phong_coeff) * diff_dot * light_color;

    // Specular reflection (Phong).
    const Vec3d reflection_direction = 2 * surface_normal.dot(light_direction) * surface_normal - light_direction;
    const double spec_dot = std::max(0.0, reflection_direction.dot(view_direction));
    const Vec3d specular = std::get<2>(phong_coeff) * std::pow(spec_dot, std::get<3>(phong_coeff)) * light_color;

    // Ambient is intentionally omitted here (added once in computeDirectLighting).
    return (diffuse + specular) * light_intensity;
    // END TODO 2
}

/**
 * @brief Method to check a ray for intersections with any object of the scene.
 * @param ray The ray to trace.
 * @param objects Vector of pointers to all scene objects.
 * @param t_near The intersection distance from the ray origin to the closest point hit.
 * @param hitObject The closest object hit.
 * @return true on hit, false otherwise
 */
bool trace(const Ray& ray,
    const std::vector<std::shared_ptr<SceneObject>>& objects,
    double& t_near, std::shared_ptr<SceneObject>& hitObject)
{
    t_near = std::numeric_limits<double>::max();

    // Check all objects if they got hit by the traced ray.
    // If any object got hit, return the one closest to the camera as 'hitObject'.
    for (auto& o : objects)
    {
        double t = std::numeric_limits<double>::max();

        if (o->intersect(ray, t) && t < t_near)
        {
            hitObject = o;
            t_near = t;
        }
    }

    return (hitObject != nullptr);
}

/**
 * @brief Check whether a surface point is in shadow with respect to a light.
 *
 * Casts a shadow ray from the (slightly offset) surface point towards the light
 * and tests it against every scene object. The point is considered shadowed if
 * an intersection is found closer than the light distance.
 *
 * @param p_hit The surface point being shaded.
 * @param surface_normal The unit normal at p_hit (used to offset the ray origin).
 * @param light The point light whose visibility is being tested.
 * @param objects All scene objects that may occlude the light.
 * @return true if the light is occluded, false otherwise.
 */
bool inShadow(const Vec3d& p_hit, const Vec3d& surface_normal,
    const Pointlight& light,
    const std::vector<std::shared_ptr<SceneObject>>& objects)
{
    const Vec3d to_light = light.getPosition() - p_hit;
    const double light_distance = to_light.length();
    const Vec3d light_direction = to_light.normalize();

    Ray shadow_ray;
    shadow_ray.origin = p_hit + surface_normal * 1e-4;  // offset to avoid self-shadowing
    shadow_ray.dir = light_direction;

    std::shared_ptr<SceneObject> shadow_hit_object = nullptr;
    double t_shadow = light_distance;

    return trace(shadow_ray, objects, t_shadow, shadow_hit_object) &&
           t_shadow < light_distance;
}

/**
 * @brief Compute the direct (local) Phong lighting at a surface point.
 *
 * Sums the diffuse and specular contributions of every light that is visible
 * from p_hit (i.e. not occluded, as determined by inShadow()). The ambient term
 * k_a is added exactly ONCE, outside the per-light loop, instead of being
 * re-added for every light - which previously caused ambient double-counting
 * and an over-bright image.
 *
 * @param ray The incoming ray (used to derive the view direction toward the camera).
 * @param p_hit The shaded surface point.
 * @param surface_normal The unit normal at p_hit.
 * @param coeffs Phong coefficients (k_a, k_d, k_s, n) of the hit surface.
 * @param lights All point lights in the scene.
 * @param objects All scene objects (used for shadow tests).
 * @return The accumulated direct-lighting color at p_hit (ambient added once).
 */
Vec3d computeDirectLighting(const Ray& ray, const Vec3d& p_hit, const Vec3d& surface_normal,
    const PhongCoefficients& coeffs, const std::vector<Pointlight>& lights,
    const std::vector<std::shared_ptr<SceneObject>>& objects)
{
    // Ambient term: added ONCE, independent of any point light (standard Phong).
    // k_a encodes the surface's ambient reflectance (equal to the surface color
    // for both Plane and Sphere in this scene).
    Vec3d result = std::get<0>(coeffs);

    const Vec3d view_direction = (ray.origin - p_hit).normalize();

    for (const auto& light : lights)
    {
        if (inShadow(p_hit, surface_normal, light, objects))
            continue;

        const Vec3d to_light = light.getPosition() - p_hit;
        const double light_distance = to_light.length();
        const Vec3d light_direction = to_light.normalize();

        result += computePhongLighting(view_direction, surface_normal, light_direction,
            coeffs, light.getColor(),
            light.getIntensity() / (light_distance * light_distance));
    }

    return result;
}

/**
 * @brief Cast a ray into the scene and shade the closest hit, if any.
 *
 * Pipeline:
 *   1. Stop recursion at MAX_DEPTH (return background color).
 *   2. Trace the ray against all objects; if nothing is hit, return the
 *      dark-blue background color.
 *   3. Compute direct Phong lighting (ambient added once, plus diffuse and
 *      specular from every non-occluded light) via computeDirectLighting().
 *   4. If the surface is reflective (k_s != 0), spawn a reflection ray and
 *      add its (recursively shaded) contribution weighted by k_s.
 *
 * @param ray The ray that's being cast.
 * @param objects All scene objects.
 * @param lights All point lights in the scene.
 * @return The shaded color at the closest hit, or dark blue if nothing was hit
 *         or MAX_DEPTH was exceeded.
 */
Vec3d castRay(const Ray& ray, const std::vector<std::shared_ptr<SceneObject>>& objects,
    const std::vector<Pointlight>& lights)
{
    // set the background color as dark blue
    Vec3d hitColor(0, 0, 0.2);

    // early exit if maximum recursive depth is reached - return background color
    if (ray.depth > MAX_DEPTH)
        return hitColor;

    // pointer to the object that was hit by the ray
    std::shared_ptr<SceneObject> hitObject = nullptr;

    // intersection distance from the ray origin to the point hit
    double t = std::numeric_limits<double>::max();

    // Trace the ray. If an object gets hit, calculate the hit point and
    // retrieve the surface properties from the 'hitObject' that was hit.
    if (trace(ray, objects, t, hitObject))
    {
        // Intersection point with the hit object
        const Vec3d p_hit = ray.origin + ray.dir * t;

        const Vec3d surface_normal = hitObject->getSurfaceNormal(p_hit);
        const PhongCoefficients phong_coeffs = hitObject->getPhongCoefficients(p_hit);

        // Local (direct) lighting: ambient once + diffuse/specular per visible light.
        hitColor = computeDirectLighting(ray, p_hit, surface_normal, phong_coeffs, lights, objects);

        // Specular reflection: spawn a reflection ray and recurse.
        if (std::get<2>(phong_coeffs).length() > 0)
        {
            const Vec3d reflection_direction =
                (2 * surface_normal.dot(-ray.dir) * surface_normal + ray.dir).normalize();

            Ray reflection_ray;
            reflection_ray.origin = p_hit + surface_normal * 1e-4;
            reflection_ray.dir = reflection_direction;
            reflection_ray.depth = ray.depth + 1;

            hitColor += std::get<2>(phong_coeffs) * castRay(reflection_ray, objects, lights);
        }
    }

    return hitColor;
}

/**
 * @brief The rendering method, loop over all pixels in the framebuffer, shooting
 *        a ray through each pixel with the origing being the camera position.
 * @param viewport Size of the framebuffer.
 * @param objects Vector of pointers to all objects contained in the scene.
 */
void render(const Vec3i viewport, const std::vector<std::shared_ptr<SceneObject>>& objects,
    const std::vector<Pointlight>& lights)
{
    std::vector<Vec3d> framebuffer(static_cast<size_t>(viewport[0]) * viewport[1]);

    // camera position in world coordinates
    const Vec3d cameraPos(0., 0., 0.);

    // view plane parameters
    const double l = -1.;   // left
    const double r = +1.;   // right
    const double b = -1.;   // bottom
    const double t = +1.;   // top
    const double d = +2.;   // distance to camera

    // Cast a ray from 'cameraPos' through the center(!) of each pixel on the viewplane.
    // Use the view plane parametrization given above (l,r,b,t,d).
    #pragma omp parallel for
    for (int j = 0; j < viewport[1]; ++j)
    {
        for (int i = 0; i < viewport[0]; ++i)
        {
            double u = l + (r - l) * (i + 0.5) / viewport[0];
            double v = t + (b - t) * (j + 0.5) / viewport[1];

            Ray ray;
            ray.origin = cameraPos;
            ray.dir = Vec3d(u, v, -d) - cameraPos;
            ray.dir = ray.dir.normalize();
            framebuffer.at(i + j * static_cast<size_t>(viewport[0])) = castRay(ray, objects, lights);
        }
    }

    // save the framebuffer an a PPM image
    saveAsPPM("./result.ppm", viewport, framebuffer);
}

/**
 * @brief main routine.
 *        Generates the scene and invokes the rendering.
 * @return
 */
int main()
{
    // Generate the scene objects
    const auto objects = create_scene_objects();

    // Let there be light
    const auto lights = create_scene_lights();

    // Start rendering
    const Vec3i viewport(WIDTH, HEIGHT, 0);
    render(viewport, objects, lights);

    return 0;
}
