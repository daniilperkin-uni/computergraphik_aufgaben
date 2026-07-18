#pragma once

#include "sceneobject.h"

#include <memory>
#include <vector>

/**
 * @brief Create a scene with a plane and a bunch of colored spheres.
 * @return The scene as list of scene objects.
 */
std::vector<std::shared_ptr<SceneObject>> create_scene()
{
    std::vector<std::shared_ptr<SceneObject>> objects;

    // Create one plane
    Vec3d planeNormal(0.0, 1.0, 0.0);
    planeNormal.normalize();

    std::shared_ptr<SceneObject> plane = std::make_shared<Plane>(Vec3d(0.0, -1.0, 5.0), planeNormal);
    objects.push_back(std::shared_ptr<SceneObject>(plane));

    // Create a bunch of colored spheres
    {
        const Vec3d pos(2.79691, -3.16565, -14.9654);
        const double radius = 0.59685;
        const Vec3d color(0.680215, 0.3897, 0.0832257);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-0.407511, -4.00025, -11.4583);
        const double radius = 0.333709;
        const Vec3d color(0.231187, 0.899334, 0.132472);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-4.43588, 1.50888, -8.42867);
        const double radius = 0.721999;
        const Vec3d color(0.327648, 0.336679, 0.533702);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(4.92212, -4.99221, -16.3855);
        const double radius = 0.617482;
        const Vec3d color(0.0900409, 0.545919, 0.940942);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-4.76938, -4.92934, -13.1165);
        const double radius = 0.524775;
        const Vec3d color(0.889082, 0.818827, 0.376314);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(4.73756, -4.53334, -10.9986);
        const double radius = 0.232771;
        const Vec3d color(0.69184, 0.131286, 0.933796);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-1.17538, 1.18386, -7.90606);
        const double radius = 0.983231;
        const Vec3d color(0.224297, 0.147904, 0.61634);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(1.80308, 3.5994, -11.6676);
        const double radius = 0.450499;
        const Vec3d color(0.471878, 0.436242, 0.30572);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(0.632882, 4.42202, -7.13265);
        const double radius = 0.385417;
        const Vec3d color(0.0638121, 0.538282, 0.832521);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-2.58975, -2.69106, -7.15966);
        const double radius = 0.683264;
        const Vec3d color(0.340974, 0.597084, 0.282512);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-3.26635, 3.33195, -13.1);
        const double radius = 0.391061;
        const Vec3d color(0.8033, 0.364325, 0.509903);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-0.748441, 2.55361, -8.82236);
        const double radius = 0.207942;
        const Vec3d color(0.629547, 0.482853, 0.0628588);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(3.42285, -4.68687, -12.677);
        const double radius = 0.449754;
        const Vec3d color(0.16608, 0.851683, 0.916801);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(2.27272, 4.26659, -10.9515);
        const double radius = 0.326541;
        const Vec3d color(0.2159, 0.950424, 0.299693);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(4.61172, 0.208343, -12.7044);
        const double radius = 0.844534;
        const Vec3d color(0.235393, 0.68133, 0.814113);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(0.867512, 0.396921, -14.4732);
        const double radius = 0.965255;
        const Vec3d color(0.657187, 0.699725, 0.713496);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-2.03726, -2.24001, -13.0703);
        const double radius = 0.165267;
        const Vec3d color(0.937254, 0.918642, 0.338299);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-1.05118, -0.765985, -7.15636);
        const double radius = 0.293488;
        const Vec3d color(0.698401, 0.0248872, 0.832002);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(2.11342, -3.01158, -7.1408);
        const double radius = 0.790176;
        const Vec3d color(0.14572, 0.903395, 0.800197);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(1.51077, 4.26301, -13.0596);
        const double radius = 0.91496;
        const Vec3d color(0.844889, 0.495297, 0.660831);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-4.0459, -0.505493, -15.5004);
        const double radius = 0.370818;
        const Vec3d color(0.565382, 0.340108, 0.676196);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(0.912978, 1.65922, -13.6884);
        const double radius = 0.274722;
        const Vec3d color(0.605955, 0.560501, 0.874004);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(4.71712, -1.17073, -12.6124);
        const double radius = 0.848914;
        const Vec3d color(0.495297, 0.452428, 0.253168);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-2.43932, -2.64015, -14.2173);
        const double radius = 0.0404336;
        const Vec3d color(0.562847, 0.231306, 0.420837);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-0.606635, -3.89109, -14.1066);
        const double radius = 0.201719;
        const Vec3d color(0.497781, 0.196731, 0.467383);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(0.632756, -0.246298, -15.9576);
        const double radius = 0.695516;
        const Vec3d color(0.0286482, 0.0439981, 0.0260184);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(0.398411, 1.04417, -8.39331);
        const double radius = 0.203061;
        const Vec3d color(0.782078, 0.920722, 0.0483436);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(1.94785, 0.988655, -16.4285);
        const double radius = 0.880468;
        const Vec3d color(0.118627, 0.316963, 0.0509399);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-3.94506, -2.04366, -13.2435);
        const double radius = 0.456535;
        const Vec3d color(0.821713, 0.296983, 0.443441);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(3.8328, -0.834901, -9.1844);
        const double radius = 0.324345;
        const Vec3d color(0.177003, 0.100103, 0.0759562);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(4.06828, -1.43702, -8.22088);
        const double radius = 0.272132;
        const Vec3d color(0.535684, 0.992047, 0.599507);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }
    {
        const Vec3d pos(-1.47431, -4.9948, -13.4769);
        const double radius = 0.304781;
        const Vec3d color(0.8382, 0.174815, 0.621885);

        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(pos, radius, color)));
    }

    return objects;
}
