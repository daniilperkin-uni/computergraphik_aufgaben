#include "Cube.h"

#include <array>
#include <cmath>
#include <string>

cg::Cube::Cube(const float edgeLength, const Color& color) : SceneObject(color), edgeLength(std::abs(edgeLength))
{ }

std::string cg::Cube::getShapeName() const
{
    return "Cube";
}

cg::TriangleMesh cg::Cube::calculateMesh() const
{
    cg::TriangleMesh mesh;

    if (this->edgeLength != 0.0f)
    {
        const float edgeLengthHalf = this->edgeLength / 2.0f;

        // Create corners
        const Point3D left_bottom_front(-edgeLengthHalf, -edgeLengthHalf, -edgeLengthHalf);
        const Point3D left_bottom_back(-edgeLengthHalf, -edgeLengthHalf, edgeLengthHalf);
        const Point3D left_top_front(-edgeLengthHalf, edgeLengthHalf, -edgeLengthHalf);
        const Point3D left_top_back(-edgeLengthHalf, edgeLengthHalf, edgeLengthHalf);
        const Point3D right_bottom_front(edgeLengthHalf, -edgeLengthHalf, -edgeLengthHalf);
        const Point3D right_bottom_back(edgeLengthHalf, -edgeLengthHalf, edgeLengthHalf);
        const Point3D right_top_front(edgeLengthHalf, edgeLengthHalf, -edgeLengthHalf);
        const Point3D right_top_back(edgeLengthHalf, edgeLengthHalf, edgeLengthHalf);

        // Create front triangles
        const Triangle front_1 = {
            std::array<Triangle::Point, 3> {
                Triangle::Point { left_bottom_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color },
                Triangle::Point { left_top_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color },
                Triangle::Point { right_top_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color } } };

        const Triangle front_2 = {
            std::array<Triangle::Point, 3> {
                Triangle::Point{ left_bottom_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color },
                Triangle::Point{ right_top_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color },
                Triangle::Point{ right_bottom_front, Point3D(0.0f, 0.0f, -1.0f), SceneObject::color } } };

        // Create back triangles
        const Triangle back_1 = {
            std::array<Triangle::Point, 3> {
                Triangle::Point{ left_bottom_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color },
                Triangle::Point{ right_bottom_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color } } };

        const Triangle back_2 = {
            std::array<Triangle::Point, 3> {
                Triangle::Point{ left_bottom_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color },
                Triangle::Point{ left_top_back, Point3D(0.0f, 0.0f, 1.0f), SceneObject::color } } };

        // Create bottom triangles
        const Triangle bottom_1 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_bottom_front, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_bottom_front, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_bottom_back, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color } } };

        const Triangle bottom_2 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_bottom_front, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_bottom_back, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_bottom_back, Point3D(0.0f, -1.0f, 0.0f), SceneObject::color } } };

        // Create top triangles
        const Triangle top_1 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_top_front, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_top_back, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color } } };

        const Triangle top_2 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_top_front, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_front, Point3D(0.0f, 1.0f, 0.0f), SceneObject::color } } };

        // Create left triangles
        const Triangle left_1 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_bottom_front, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_top_front, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_top_back, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color } } };

        const Triangle left_2 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ left_bottom_front, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_top_back, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ left_bottom_back, Point3D(-1.0f, 0.0f, 0.0f), SceneObject::color } } };

        // Create right triangles
        const Triangle right_1 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ right_bottom_front, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_bottom_back, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color } } };

        const Triangle right_2 = {
            std::array<Triangle::Point, 3> {
            Triangle::Point{ right_bottom_front, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_back, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color },
                Triangle::Point{ right_top_front, Point3D(1.0f, 0.0f, 0.0f), SceneObject::color } } };

        // Add triangles to mesh
        mesh.push_back(front_1);
        mesh.push_back(front_2);
        mesh.push_back(back_1);
        mesh.push_back(back_2);
        mesh.push_back(bottom_1);
        mesh.push_back(bottom_2);
        mesh.push_back(top_1);
        mesh.push_back(top_2);
        mesh.push_back(left_1);
        mesh.push_back(left_2);
        mesh.push_back(right_1);
        mesh.push_back(right_2);
    }

    return mesh;
}
