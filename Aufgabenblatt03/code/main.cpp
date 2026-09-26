#include "scene.h"
#include "sceneobject.h"
#include "util.h"
#include "vec3.h"

#include <limits>
#include <memory>
#include <vector>

// TODO: Set the test according to your current exercise.
const static bool TEST_RAY_GENERATION = false;
const static bool TEST_SPHERE_INTERSECT = true;

constexpr int WIDTH = 600;
constexpr int HEIGHT = 600;

// Set to true when a configured comparison against a reference image failed,
// which makes main() return a non-zero exit code (used by ctest, see CMakeLists.txt).
static bool g_comparison_failed = false;

/**
 * @brief Method to check a ray for intersections with any object of the scene.
 * @param ray The ray to trace.
 * @param objects Vector of pointers to all scene objects.
 * @param t_near The intersection distance from the ray origin to the closest point hit.
 * @param hitObject The closest object hit.
 * @return true on hit, false otherwise
 */
bool trace(const Ray &ray,
           const std::vector<std::shared_ptr<SceneObject>> &objects,
           double &t_near, std::shared_ptr<SceneObject> &hitObject)
{
    ///////////
    // TODO
    // Check all objects if they got hit by the traced ray.
    // If any object got hit, return true, otherwise false.
    // If any object got hit, store the object closest to the camera in 'hitObject' and the corresponding t (r(t) = ray_origin + t * ray_direction) to the object in 't_near'.
    // END TODO
    ///////////

    // Initialisiere t_near mit dem maximalen double-Wert
    t_near = std::numeric_limits<double>::max();
    hitObject = nullptr;

    // Iteriere über alle Objekte in der Szene
    for (const auto& object : objects) {
        double t = std::numeric_limits<double>::max();

        // Prüfe, ob der Strahl das aktuelle Objekt trifft
        if (object->intersect(ray, t)) {
            // Wenn der Treffpunkt näher an der Kamera ist als alle bisherigen Treffer
            if (t < t_near) {
                t_near = t;
                hitObject = object;
            }
        }
    }

    return (hitObject != nullptr);
}

/**
 * @brief Cast a ray into the scene. If the ray hits at least one object,
 *        the color of the object closest to the camera is returned.
 * @param ray The ray that's being cast.
 * @param objects All scene objects.
 * @return The color of a hit object that is closest to the camera.
 *         Return dark blue if no object was hit.
 */
Vec3d castRay(const Ray &ray, const std::vector<std::shared_ptr<SceneObject>> &objects)
{
    // Set the background color as dark blue
    Vec3d hitColor(0.0, 0.0, 0.2);

    ///////////
    // TODO
    // Trace the ray by calling 'trace(...)'. If an object gets hit, calculate the hit point
    // and retrieve the surface color 'hitColor' from the 'hitObject'.
    // 
    // Note that the trace(...) method accepts non-const pointers to modify the provided t_near and hitObject arguments in case an object was hit.
    //
    // cf., lecture slide raytracing 10ff
    // END TODO
    ///////////

    double t_near = std::numeric_limits<double>::max();
    std::shared_ptr<SceneObject> hitObject = nullptr;

    // Rufe trace auf, um das nächstgelegene Objekt zu finden
    if (trace(ray, objects, t_near, hitObject)) {
        // Berechne den Treffpunkt
        Vec3d hitPoint = ray.origin + ray.dir * t_near;

        // Hole die Farbe des getroffenen Objekts am Treffpunkt
        hitColor = hitObject->getSurfaceColor(hitPoint);
    }

    return hitColor;
}

/**
 * @brief The rendering method, loop over all pixels in the framebuffer, shooting
 *        a ray through each pixel with the origing being the camera position.
 * @param viewport Size of the framebuffer.
 * @param objects Vector of pointers to all objects contained in the scene.
 */
void render(const Vec3i viewport, const std::vector<std::shared_ptr<SceneObject>> &objects)
{
    std::vector<Vec3d> framebuffer(static_cast<size_t>(viewport[0] * viewport[1]));

    // Camera position in world coordinates (at the origin)
    const Vec3d cameraPos(0.0, 0.0, 0.0);

    // View plane parameters (FoV: ~53.13°)
    const double l = -1.0;   // left
    const double r = +1.0;   // right
    const double b = -1.0;   // bottom
    const double t = +1.0;   // top
    const double d = +2.0;   // distance to camera

    // Iteriere über alle Pixel im Bild
    for (int j = 0; j < viewport[1]; ++j) {
        for (int i = 0; i < viewport[0]; ++i) {
            // Bildkoordinaten für die Pixelmitte auf den Bereich [0, 1] abbilden.
            double u = (double(i) + 0.5) / double(viewport[0]);
            double v = (double(j) + 0.5) / double(viewport[1]);

            // Auf die View Plane abbilden
            double x = l + (r - l) * u;
            double y = t - (t - b) * v; // y-Achse umkehren
            double z = -d;

            Vec3d rayDirection(x, y, z);
            rayDirection.normalize();

            Ray ray;
            ray.origin = cameraPos;
            ray.dir = rayDirection;

            // Berechne die Farbe für diesen Pixel
            Vec3d pixelColor = castRay(ray, objects);

            // Speichere die Farbe im Framebuffer
            int pixelIndex = j * viewport[0] + i;
            framebuffer.at(static_cast<size_t>(pixelIndex)) = pixelColor;
        }
    }

    // ... Rest der Funktion bleibt gleich ...
    saveAsPPM("./result.ppm", viewport, framebuffer);

    if (TEST_RAY_GENERATION)
    {
        g_comparison_failed =
            !comparePPM("../reference_rayGeneration.ppm", "ray generation test", framebuffer);
    }
    else if (TEST_SPHERE_INTERSECT)
    {
        g_comparison_failed =
            !comparePPM("../reference_sphereIntersection.ppm", "sphere intersection test", framebuffer);
    }
}

/**
 * @brief main routine.
 *        Generates the scene and invokes the rendering.
 * @return
 */
int main()
{
    // Generate the scene
    const auto objects = create_scene();

    // Start rendering
    const Vec3i viewport(WIDTH, HEIGHT, 0);

    render(viewport, objects);

    // Non-zero exit code when the reference comparison did not match, so the
    // build can be verified by ctest (see CMakeLists.txt).
    return g_comparison_failed ? 1 : 0;
}
