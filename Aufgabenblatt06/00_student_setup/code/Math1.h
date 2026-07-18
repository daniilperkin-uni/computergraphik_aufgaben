#pragma once

#include "glm/glm.hpp"

#include <array>
#include <vector>

namespace cg
{
    // Numeric constants
    float pi();

    // Operator for near equal values
    bool equals(float a, float b, float eps = 0.001f);

    // Types for vectors and matrices
    using ivec2 = glm::ivec2;
    using ivec3 = glm::ivec3;
    using ivec4 = glm::ivec4;

    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;

    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;

    // Specialized types
    using Color = vec4;
    using Point3D = vec3;
    using Point2D = vec2;

    // Zero and unit vectors/matrices
    vec2 zeroVec2();
    vec3 zeroVec3();
    vec4 zeroVec4();

    vec2 oneVec2();
    vec3 oneVec3();
    vec4 oneVec4();

    mat2 zeroMat2();
    mat3 zeroMat3();
    mat4 zeroMat4();

    mat2 unitMat2();
    mat3 unitMat3();
    mat4 unitMat4();

    // Color specific vectors
    Color black();
    Color white();
    Color red();
    Color green();
    Color blue();

    // Types for triangle and triangle mesh
    struct Triangle2D;

    struct Triangle
    {
        struct Point
        {
            using point_type = Point3D;
            using color_type = Color;

            point_type position;
            point_type normal;
            color_type color;

            bool validXY;
            bool validZ;
        };

        std::array<Point, 3> points;
    };

    struct Triangle2D
    {
        Triangle2D();
        Triangle2D(const Triangle& other);

        struct Point
        {
            using point_type = Point2D;
            using color_type = Color;

            point_type position;
            color_type color;
        };

        std::array<Point, 3> points;
    };

    using TriangleMesh = std::vector<Triangle>;

    // Functions for triangles
    bool pointInTriangle(const Triangle2D& triangle, const Point2D& position);
    vec3 calculateBarycentricCoords(const Triangle2D& triangle, const Point2D& position);

    // Coordinate system
    Point3D sphericalToCartesian(float r, float teta, float phi);
}