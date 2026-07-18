#include "Container.h"

#include <memory>
#include <string>

#include "glm/gtc/matrix_inverse.hpp"

void cg::Container::addObject(std::shared_ptr<SceneObject> object)
{
    this->objects.push_back(object);
}

std::string cg::Container::getShapeName() const
{
    return "Container";
}

cg::TriangleMesh cg::Container::calculateMesh() const
{
    TriangleMesh mesh;

    // Get objects' triangle meshes into container space and return them
    for (const auto& object : this->objects)
    {
        const auto transformation = object->getTransformation();
        const auto normalTransformation = glm::inverseTranspose(transformation);

        auto objectMesh = object->calculateMesh();

        for (auto& triangle : objectMesh)
        {
            triangle.points[0].position = vec3(transformation * vec4(triangle.points[0].position, 1.0f));
            triangle.points[1].position = vec3(transformation * vec4(triangle.points[1].position, 1.0f));
            triangle.points[2].position = vec3(transformation * vec4(triangle.points[2].position, 1.0f));

            triangle.points[0].normal = vec3(normalTransformation * vec4(triangle.points[0].normal, 0.0f));
            triangle.points[1].normal = vec3(normalTransformation * vec4(triangle.points[1].normal, 0.0f));
            triangle.points[2].normal = vec3(normalTransformation * vec4(triangle.points[2].normal, 0.0f));
        }

        mesh.insert(mesh.end(), objectMesh.begin(), objectMesh.end());
    }

    return mesh;
}
