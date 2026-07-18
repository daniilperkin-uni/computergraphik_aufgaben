#include "Rasterizer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

cg::Rasterizer::Rasterizer(const Camera camera, const std::vector<Scene>& scenes, const rasterization_mode mode, const unsigned int width, const unsigned int height)
    : camera(camera), scenes(scenes), mode(mode), image(width, height), zBuffer(width, height),
    lastRotation(std::chrono::milliseconds::zero()), rotationSpeed(10.0f), rotationAxis(0.0f, 0.0f, 1.0f)
{
    if (this->scenes.size() == 0)
    {
        throw std::runtime_error("There must be at least one scene!");
    }

    this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
}

void cg::Rasterizer::draw(const bool rotate)
{
    // Reset image and z-buffer
    this->image.initialize(0.0f);
    this->zBuffer.resize(this->image.get_width(), this->image.get_height());
    this->zBuffer.initialize(std::numeric_limits<float>::max());

    // Rotate scene in front of the camera
    auto alpha = 0.0f;
    vec3 rotationAxis = oneVec3();

    if (rotate)
    {
        std::chrono::milliseconds current = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        if (this->lastRotation == std::chrono::milliseconds::zero())
        {
            this->lastRotation = current;
        }
        else
        {
            alpha = this->rotationSpeed * static_cast<float>((current - this->lastRotation).count()) / 1000.0f;
        }

        rotationAxis = this->rotationAxis;
    }
    else
    {
        this->lastRotation = std::chrono::milliseconds::zero();
    }

    const auto transformation = glm::rotate(unitMat4(), alpha, rotationAxis);

    // Information message
    switch (this->mode)
    {
    case POINTS:
        std::cout << "Drawing points..." << std::endl;

        break;
    case WIREFRAME:
        std::cout << "Drawing wireframe..." << std::endl;

        break;
    case FILLED:
        std::cout << "Drawing triangles..." << std::endl;
    }

    // Draw each object
    for (const auto obj : this->scenes[this->activeScene].getObjects())
    {
        if (obj->isVisible())
        {
            drawObject(obj, transformation);
        }
    }
}

cg::Camera& cg::Rasterizer::accessCamera()
{
    return this->camera;
}

cg::Scene& cg::Rasterizer::accessScene()
{
    return this->scenes[this->activeScene];
}

const std::vector<cg::Scene>& cg::Rasterizer::getScenes() const
{
    return this->scenes;
}

int& cg::Rasterizer::accessActiveScene()
{
    return this->activeScene;
}

int cg::Rasterizer::getNumScenes() const
{
    return static_cast<int>(this->scenes.size());
}

cg::Rasterizer::rasterization_mode& cg::Rasterizer::accessMode()
{
    return this->mode;
}

cg::image<cg::color_space_t::RGBA>& cg::Rasterizer::accessImage()
{
    return this->image;
}

float& cg::Rasterizer::accessRotationSpeed()
{
    return this->rotationSpeed;
}

cg::vec3& cg::Rasterizer::accessRotationAxis()
{
    return this->rotationAxis;
}

void cg::Rasterizer::drawObject(const std::shared_ptr<cg::SceneObject> object, const mat4& transformation)
{
    // Ger camera transformations
    const auto viewProjection = this->camera.getViewProjection();

    // Draw the mesh
    const auto mesh = object->calculateMesh();
    const auto model = object->getTransformation();

    // Get lights
    auto lights = this->scenes[this->activeScene].getLights();

    // For each triangle
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < mesh.size(); ++i)
    {
        auto triangle = mesh[i];

        // For each triangle corner point
        for (auto& point : triangle.points)
        {
            // Transform to world space
            const auto global_trafo = transformation * model;

            ///////
            // TODO
            // Transform the position of the point and the normal to world space.
            // Transform position from object space to world space using the global transformation matrix.
            vec4 point_world_homogeneous = global_trafo * vec4(point.position, 1.0f);
            // Store the homogeneous world-space position in point_world
            vec4 point_world = point_world_homogeneous;

            // Transform normal from object space to world space using the inverse-transpose of the 3x3 global transformation matrix.
            mat3 normal_matrix = glm::inverseTranspose(mat3(global_trafo));
            vec3 normal_world = glm::normalize(normal_matrix * point.normal);


            // Calculate lighting
            auto color = black();

            for (const auto& light : lights)
            {
                if (light->isVisible())
                {
                    const auto colorInfo = light->getColor(Point3D(point_world));

                    if (colorInfo.ambient)
                    {
                        // Add ambient part
                        color += point.color * colorInfo.color * colorInfo.intensity;
                    }
                    else
                    {
                        // Get the light's position in world space.
                        // The position is derived from the light's transformation matrix (translation component).
                        // Note: The `getColor` function in PointLight calculates `obj - position`,
                        // where `position` is the light's world position.
                        const vec3 lightPos = Point3D(light->getTransformation() * vec4(0.0f, 0.0f, 0.0f, 1.0f));

                        // Normalize surface normal (already in world space from previous step)
                        const vec3 N = glm::normalize(normal_world);

                        // Calculate light direction vector L (from object to light)
                        // It's the inverse of colorInfo.ray, which is from light to object.
                        const vec3 L = glm::normalize(lightPos - vec3(point_world));

                        // Calculate Lambert term: max(0.0f, dot(N, L))
                        // This determines how much diffuse light is reflected based on the angle between normal and light direction.
                        float lambertTerm = glm::max(0.0f, glm::dot(N, L));

                        // Calculate distance from light source to the surface point
                        float d = glm::distance(vec3(point_world), lightPos);

                        // Calculate attenuation factor: 1.0 / (0.001 + d^2)
                        // This simulates light dimming with distance from the source.
                        float attenuation = 1.0f / (0.001f + d * d);

                        // Apply intensity to RGB channels of light color, preserving alpha channel.
                        // The alpha channel of the light color remains unaffected by intensity.
                        Color lightColorWithIntensity;
                        lightColorWithIntensity.r = colorInfo.color.r * colorInfo.intensity;
                        lightColorWithIntensity.g = colorInfo.color.g * colorInfo.intensity;
                        lightColorWithIntensity.b = colorInfo.color.b * colorInfo.intensity;
                        lightColorWithIntensity.a = colorInfo.color.a; 

                        // Combine all terms to get the diffuse component of the light.
                        // Final Color calculation: LightColor.rgb * Intensity * LambertTerm * Attenuation.
                        Color diffuseLight = lightColorWithIntensity * lambertTerm * attenuation;

                        // Add the calculated diffuse light to the total accumulated color for the point.
                        color += diffuseLight;
                    }
                }
            }

            point.color = color;

            // Transform to clip space
            auto point_clip = viewProjection * point_world;

            point.validXY = point_clip.x > -point_clip.w && point_clip.x < point_clip.w&& point_clip.y > -point_clip.w && point_clip.y < point_clip.w;
            point.validZ = point_clip.z > -point_clip.w && point_clip.z < point_clip.w;

            // Transform to screen space
            point_clip.x = point_clip.x / point_clip.w;
            point_clip.y = point_clip.y / point_clip.w;
            point_clip.z = point_clip.z / point_clip.w;

            point.position.x = (point_clip.x * 0.5f + 0.5f) * this->image.get_width();
            point.position.y = (point_clip.y * -0.5f + 0.5f) * this->image.get_height();
            point.position.z = point_clip.z;
        }

        // Draw triangle
        drawTriangle(triangle);
    }
}

void cg::Rasterizer::drawTriangle(const Triangle& triangle)
{
    switch (this->mode)
    {
    case POINTS:
        rasterizePoints(triangle);

        break;
    case WIREFRAME:
        drawWireframe(triangle);

        break;
    case FILLED:
        rasterizeFilled(triangle);
    }
}

void cg::Rasterizer::rasterizePoints(const Triangle& triangle)
{
    // Draw pixels if they are in front of the camera
    if (triangle.points[0].validXY && triangle.points[0].validZ) setPixel(triangle.points[0].position, triangle.points[0].color);
    if (triangle.points[1].validXY && triangle.points[1].validZ) setPixel(triangle.points[1].position, triangle.points[1].color);
    if (triangle.points[2].validXY && triangle.points[2].validZ) setPixel(triangle.points[2].position, triangle.points[2].color);
}

void cg::Rasterizer::drawWireframe(const Triangle& triangle)
{
    // Create lines
    const std::array<std::pair<Triangle::Point, Triangle::Point>, 3> lines = {
        std::make_pair(triangle.points[0], triangle.points[1]),
        std::make_pair(triangle.points[0], triangle.points[2]),
        std::make_pair(triangle.points[1], triangle.points[2]) };

    for (const auto& line : lines)
    {
        if ((line.first.validXY && line.first.validZ) || (line.second.validXY && line.second.validZ))
        {
            // Rasterize the line
            rasterizeLine(line.first, line.second);
        }
    }
}

void cg::Rasterizer::rasterizeFilled(const Triangle& triangle)
{
    if (triangle.points[0].validZ && triangle.points[1].validZ && triangle.points[2].validZ)
    {
        // Line-wise fill triangle
        const auto x_min = static_cast<unsigned int>(std::max(static_cast<int>(std::round(std::min(
            { triangle.points[0].position.x, triangle.points[1].position.x, triangle.points[2].position.x }))), 0));
        const auto x_max = static_cast<unsigned int>(std::max(std::min(static_cast<int>(std::round(std::max(
            { triangle.points[0].position.x, triangle.points[1].position.x, triangle.points[2].position.x }))), static_cast<int>(this->image.get_width() - 1)), 0));

        const auto y_min = static_cast<unsigned int>(std::max(static_cast<int>(std::round(std::min(
            { triangle.points[0].position.y, triangle.points[1].position.y, triangle.points[2].position.y }))), 0));
        const auto y_max = static_cast<unsigned int>(std::max(std::min(static_cast<int>(std::round(std::max(
            { triangle.points[0].position.y, triangle.points[1].position.y, triangle.points[2].position.y }))), static_cast<int>(this->image.get_height() - 1)), 0));

        for (auto y = y_min; y <= y_max; ++y)
        {
            for (auto x = x_min; x <= x_max; ++x)
            {
                // Check whether the x, y coordinates are inside of the triangle
                const Triangle2D triangle_2d(triangle);
                const vec2 coords(x, y);

                if (pointInTriangle(triangle_2d, coords))
                {
                    // Calculate interpolated z-value and color
                    const auto weights = calculateBarycentricCoords(triangle_2d, coords);

                    const auto z = weights[0] * triangle.points[0].position.z + weights[1] * triangle.points[1].position.z + weights[2] * triangle.points[2].position.z;
                    const auto color = weights[0] * triangle.points[0].color + weights[1] * triangle.points[1].color + weights[2] * triangle.points[2].color;

                    setPixel(Point3D(static_cast<float>(x), static_cast<float>(y), z), color);
                }
            }
        }
    }
}

void cg::Rasterizer::rasterizeLine(const cg::Triangle::Point& point_start, const cg::Triangle::Point& point_end)
{
    // Extract screen coordinates and color/z values from start and end points
    // Use integer coordinates for Bresenham's algorithm to ensure precision.
    int x_start = static_cast<int>(std::round(point_start.position.x));
    int y_start = static_cast<int>(std::round(point_start.position.y));
    float z_start = point_start.position.z;
    Color color_start = point_start.color;

    int x_end = static_cast<int>(std::round(point_end.position.x));
    int y_end = static_cast<int>(std::round(point_end.position.y));
    float z_end = point_end.position.z;
    Color color_end = point_end.color;

    // --- Bresenham's algorithm starts here ---

    bool steep = false;
    // Handle steep lines by swapping x and y coordinates.
    // This ensures that the primary iteration is always along the axis with the greater change (less steep slope).
    // The swapped coordinates will be unswapped before drawing the pixel.
    if (glm::abs(y_end - y_start) > glm::abs(x_end - x_start))
    {
        std::swap(x_start, y_start);
        std::swap(x_end, y_end);
        steep = true;
    }

    // Ensure the line is drawn from left to right (x_start <= x_end).
    // If x_start > x_end, swap start and end points along with their associated attributes (z and color).
    if (x_start > x_end)
    {
        std::swap(x_start, x_end);
        std::swap(y_start, y_end);
        std::swap(z_start, z_end);
        std::swap(color_start, color_end);
    }

    int dx = x_end - x_start;
    int dy = glm::abs(y_end - y_start);
    
    // Initial error term (decision parameter).
    // Used to decide whether to increment y or keep it the same for the next pixel.
    int error = 2 * dy - dx;
    
    // Determine the direction to step in the y-coordinate.
    // This handles lines drawn from bottom to top or top to bottom.
    int ystep = (y_start < y_end) ? 1 : -1;

    int y = y_start;

    // Iterate along the x-axis (which might be the original y-axis if 'steep' is true).
    for (int x = x_start; x <= x_end; ++x)
    {
        // Calculate the actual screen coordinates for the pixel to be drawn.
        // If 'steep' is true, swap x and y back to their original orientation.
        int current_x = steep ? y : x;
        int current_y = steep ? x : y;
        
        // Calculate the interpolation factor 't'.
        // 't' represents the progress along the line from the start point (t=0.0f) to the end point (t=1.0f).
        // This is crucial for interpolating z and color values accurately.
        float t;
        if (dx == 0) { // Special case for vertical lines (after potential swap, dx is 0 only if it was originally a vertical line)
             if (dy == 0) t = 0.0f; // Single point if both dx and dy are 0
             else t = static_cast<float>(glm::abs(y - y_start)) / static_cast<float>(glm::abs(y_end - y_start)); // Interpolate based on y-progress
        } else {
            t = static_cast<float>(x - x_start) / static_cast<float>(dx);
        }
        
        // Interpolate z and color values using 't'.
        float interpolated_z = z_start + t * (z_end - z_start);
        Color interpolated_color = color_start + t * (color_end - color_start);

        // Draw the pixel at the calculated screen coordinates with interpolated z and color.
        setPixel(Point3D(static_cast<float>(current_x), static_cast<float>(current_y), interpolated_z), interpolated_color);

        // Update the error term and y-coordinate for the next iteration.
        // If the error is negative, it means the ideal line has crossed the midpoint,
        // so increment/decrement y and reset the error term.
        if (error < 0)
        {
            y += ystep;
            error += 2 * dx; // Reset error by adding 2*dx
        }
        error -= 2 * dy; // Always decrease error by 2*dy
    }

} // End of rasterizeLine function

void cg::Rasterizer::setPixel(const Point3D& point, Color color)
{
    const auto x = static_cast<int>(std::round(point.x));
    const auto y = static_cast<int>(std::round(point.y));
    const auto z = point.z;

#pragma omp critical(image_access)
    {
        // Only draw if:
        //  - (x, y) coordinates are within the image
        //  - z is within the camera's range [near, far]
        //  - z is smaller than or equal to the one stored in the z-buffer
        if (x >= 0 && x < static_cast<int>(this->image.get_width()) && y >= 0 && y < static_cast<int>(this->image.get_height())
            && z > this->camera.getNear() && z < this->camera.getFar() && this->zBuffer.at(x, y)[0] >= z)
        {
            // If z-buffer equals z-value, only draw pixel if it is lighter than the current one
            if (this->zBuffer.at(x, y)[0] == z)
            {
                const float r_weight = 0.2989f;
                const float g_weight = 0.5870f;
                const float b_weight = 0.1140f;

                const auto luminance_old = r_weight * this->image.at(x, y)[0] + g_weight * this->image.at(x, y)[1] + b_weight * this->image.at(x, y)[2];
                const auto luminance_new = r_weight * color.r + g_weight * color.g + b_weight * color.b;

                if (luminance_new < luminance_old)
                {
                    color = vec4(this->image.at(x, y)[0], this->image.at(x, y)[1], this->image.at(x, y)[2], this->image.at(x, y)[3]);
                }
            }

            this->image.at(x, y) = { color.r, color.g, color.b, color.a };
            this->zBuffer.at(x, y)[0] = z;
        }
    }
}

