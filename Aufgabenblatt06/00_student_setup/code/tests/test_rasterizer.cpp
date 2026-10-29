// Regression test for the rasterizer depth test (QA-002).
//
// Triangle::Point::position.z holds the normalized device coordinate after the
// perspective divide, so it must not be compared against the view-space near
// and far distances. With near = 1.0 the old check dropped every fragment of
// the test triangle even though it sits well inside the frustum.
#include "Rasterizer.h"

#include "scene/Camera.h"
#include "scene/Lights/AmbientLight.h"
#include "scene/Objects/Triangle.h"
#include "scene/Scene.h"

#include <iostream>
#include <memory>
#include <vector>

int main()
{
    const cg::Camera camera(60.0f, 1.0f, 1.0f, 100.0f,
                            cg::zeroVec3(), cg::Point3D(0.0f, 0.0f, 1.0f), cg::vec3(0.0f, 1.0f, 0.0f));

    cg::Scene scene("depth test");
    scene.addObject(std::make_shared<cg::TestTriangle>());
    scene.addLight(std::make_shared<cg::AmbientLight>(cg::white(), 1.0f));

    std::vector<cg::Scene> scenes;
    scenes.push_back(scene);

    cg::Rasterizer rasterizer(camera, scenes, cg::Rasterizer::FILLED, 64, 64);
    rasterizer.draw(false);

    auto& image = rasterizer.accessImage();

    unsigned int drawn_pixels = 0;

    for (unsigned int j = 0; j < image.get_height(); ++j)
    {
        for (unsigned int i = 0; i < image.get_width(); ++i)
        {
            if (image(i, j)[0] != 0.0f || image(i, j)[1] != 0.0f || image(i, j)[2] != 0.0f)
            {
                ++drawn_pixels;
            }
        }
    }

    std::cout << "drawn pixels: " << drawn_pixels << '\n';

    if (drawn_pixels == 0)
    {
        std::cerr << "FAIL: the test triangle at view depth 5 was culled with near = 1.0\n";
        return 1;
    }

    std::cout << "Rasterizer depth test passed\n";
    return 0;
}
