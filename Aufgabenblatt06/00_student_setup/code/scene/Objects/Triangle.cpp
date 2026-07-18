#include "Triangle.h"

#include <string>

std::string cg::TestTriangle::getShapeName() const
{
    return "Triangle";
}

cg::TriangleMesh cg::TestTriangle::calculateMesh() const
{
    cg::TriangleMesh mesh(1);

    // Red corner
    mesh[0].points[0].position.x = -1.0f;
    mesh[0].points[0].position.y = -1.0f;
    mesh[0].points[0].position.z = 5.0f;

    mesh[0].points[0].normal.x = 0.0f;
    mesh[0].points[0].normal.y = 0.0f;
    mesh[0].points[0].normal.z = 1.0f;

    mesh[0].points[0].color.r = 1.0f;
    mesh[0].points[0].color.g = 0.0f;
    mesh[0].points[0].color.b = 0.0f;
    mesh[0].points[0].color.a = 1.0f;

    // Green corner
    mesh[0].points[1].position.x = 1.0f;
    mesh[0].points[1].position.y = -1.0f;
    mesh[0].points[1].position.z = 5.0f;

    mesh[0].points[1].normal.x = 0.0f;
    mesh[0].points[1].normal.y = 0.0f;
    mesh[0].points[1].normal.z = 1.0f;

    mesh[0].points[1].color.r = 0.0f;
    mesh[0].points[1].color.g = 1.0f;
    mesh[0].points[1].color.b = 0.0f;
    mesh[0].points[1].color.a = 1.0f;

    // Blue corner
    mesh[0].points[2].position.x = 0.0f;
    mesh[0].points[2].position.y = 2.0f;
    mesh[0].points[2].position.z = 5.0f;

    mesh[0].points[2].normal.x = 0.0f;
    mesh[0].points[2].normal.y = 0.0f;
    mesh[0].points[2].normal.z = 1.0f;

    mesh[0].points[2].color.r = 0.0f;
    mesh[0].points[2].color.g = 0.0f;
    mesh[0].points[2].color.b = 1.0f;
    mesh[0].points[2].color.a = 1.0f;

    return mesh;
}
