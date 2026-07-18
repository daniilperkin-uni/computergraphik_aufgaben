#include "Sphere.h"

#include <string>

cg::Sphere::Sphere(const float radius, const unsigned int tetaResolution, const unsigned int phiResolution, const Color& color)
    : SceneObject(color), radius(radius), tetaResolution(tetaResolution), phiResolution(phiResolution)
{ }

std::string cg::Sphere::getShapeName() const
{
    return "Sphere";
}

cg::TriangleMesh cg::Sphere::calculateMesh() const
{
    TriangleMesh mesh;

    const float tetaStep = pi() / this->tetaResolution;
    const float phiStep = 2.0f * pi() / this->phiResolution;

    for (unsigned int phiIndex = 0; phiIndex < this->phiResolution; ++phiIndex)
    {
        const float phiLeft = phiIndex * phiStep;
        const float phiRight = ((phiIndex + 1) % this->phiResolution) * phiStep;

        const vec3 triangleNormal_1 = sphericalToCartesian(1.0f, tetaStep, phiLeft);
        const vec3 triangleNormal_2 = sphericalToCartesian(1.0f, tetaStep, phiRight);
        const vec3 triangleNormal_3 = sphericalToCartesian(1.0f, 0.0f, 0.0f);

        const Triangle::Point trianglePoint_1 = { triangleNormal_1 * this->radius, triangleNormal_1, SceneObject::color };
        const Triangle::Point trianglePoint_2 = { triangleNormal_2 * this->radius, triangleNormal_2, SceneObject::color };
        const Triangle::Point trianglePoint_3 = { triangleNormal_3 * this->radius, triangleNormal_3, SceneObject::color };

        mesh.push_back(Triangle{ trianglePoint_1, trianglePoint_2, trianglePoint_3 });

        for (unsigned int tetaIndex = 1; tetaIndex < this->tetaResolution - 1; ++tetaIndex)
        {
            const float tetaTop = tetaIndex * tetaStep;
            const float tetaBottom = ((tetaIndex + 1) % this->tetaResolution) * tetaStep;

            const vec3 quadNormal_1 = sphericalToCartesian(1.0f, tetaTop, phiLeft);
            const vec3 quadNormal_2 = sphericalToCartesian(1.0f, tetaTop, phiRight);
            const vec3 quadNormal_3 = sphericalToCartesian(1.0f, tetaBottom, phiLeft);
            const vec3 quadNormal_4 = sphericalToCartesian(1.0f, tetaBottom, phiRight);

            const Triangle::Point quadPoint_1 = { quadNormal_1 * this->radius, quadNormal_1, SceneObject::color };
            const Triangle::Point quadPoint_2 = { quadNormal_2 * this->radius, quadNormal_2, SceneObject::color };
            const Triangle::Point quadPoint_3 = { quadNormal_3 * this->radius, quadNormal_3, SceneObject::color };
            const Triangle::Point quadPoint_4 = { quadNormal_4 * this->radius, quadNormal_4, SceneObject::color };

            mesh.push_back(Triangle{ quadPoint_1, quadPoint_3, quadPoint_2 });
            mesh.push_back(Triangle{ quadPoint_3, quadPoint_4, quadPoint_2 });
        }

        const vec3 triangleNormal_4 = sphericalToCartesian(1.0f, pi() - tetaStep, phiLeft);
        const vec3 triangleNormal_5 = sphericalToCartesian(1.0f, pi() - tetaStep, phiRight);
        const vec3 triangleNormal_6 = sphericalToCartesian(1.0f, pi(), 0.0f);

        const Triangle::Point trianglePoint_4 = { triangleNormal_4 * this->radius, triangleNormal_4, SceneObject::color };
        const Triangle::Point trianglePoint_5 = { triangleNormal_5 * this->radius, triangleNormal_5, SceneObject::color };
        const Triangle::Point trianglePoint_6 = { triangleNormal_6 * this->radius, triangleNormal_6, SceneObject::color };

        mesh.push_back(Triangle{ trianglePoint_4, trianglePoint_5, trianglePoint_6 });
    }

    return mesh;
}
