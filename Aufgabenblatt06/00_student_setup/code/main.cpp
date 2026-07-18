#include "scene/Camera.h"
#include "scene/Lights/AmbientLight.h"
#include "scene/Lights/PointLight.h"
#include "scene/Scene.h"
#include "scene/Objects/Container.h"
#include "scene/Objects/Cube.h"
#include "scene/Objects/Triangle.h"
#include "scene/Objects/Sphere.h"
#include "Rasterizer.h"

#include "image/ImageViewer.h"

#include <iostream>
#include <memory>
#include <vector>
#include <cmath> // Wichtig für std::sin und std::cos

// --- Bestehende Szenen (unverändert) ---

cg::Scene createTriangle()
{
    cg::Scene scene("Triangle");
    scene.addObject(std::make_shared<cg::TestTriangle>());
    scene.addLight(std::make_shared<cg::AmbientLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f));
    return scene;
}

cg::Scene createCube()
{
    cg::Scene scene("Cube");
    auto cube = std::make_shared<cg::Cube>();
    cube->Translate(cg::vec3(0.0f, -1.0f, 5.0f));
    cube->Rotate(0.0f, 45.0f, 0.0f);
    scene.addObject(cube);

    auto pointLight = std::make_shared<cg::PointLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 10.0f);
    pointLight->Translate(cg::vec3(-2.0f, 1.0f, 0.0f));
    scene.addLight(pointLight);

    auto ambient = std::make_shared<cg::AmbientLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.2f);
    scene.addLight(ambient);
    return scene;
}

cg::Scene createSphere()
{
    cg::Scene scene("Sphere");
    auto sphere = std::make_shared<cg::Sphere>(1.0f);
    sphere->Translate(cg::vec3(0.0f, 0.0f, 5.0f));
    sphere->Rotate(30.0f, 20.0f, 10.0f);
    scene.addObject(sphere);

    auto pointLight = std::make_shared<cg::PointLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 10.0f);
    pointLight->Translate(cg::vec3(-2.0f, 1.0f, 0.0f));
    scene.addLight(pointLight);

    auto ambient = std::make_shared<cg::AmbientLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.2f);
    scene.addLight(ambient);
    return scene;
}

cg::Scene createComplexScene()
{
    cg::Scene scene("Complex Scene");
    auto cube = std::make_shared<cg::Cube>(0.5f, cg::Color(1.0f, 1.0f, 1.0f, 1.0f));
    auto sphere1 = std::make_shared<cg::Sphere>(0.25f, 10, 20, cg::Color(1.0f, 1.0f, 0.0f, 1.0f));
    sphere1->Translate(cg::vec3(-0.5f, 0.0f, 0.0f));
    auto sphere2 = std::make_shared<cg::Sphere>(0.25f, 25, 50, cg::Color(0.0f, 1.0f, 1.0f, 1.0f));
    sphere2->Translate(cg::vec3(0.5f, 0.0f, 0.0f));

    auto object1 = std::make_shared<cg::Container>();
    object1->addObject(cube);
    object1->addObject(sphere1);
    object1->addObject(sphere2);
    object1->Translate(cg::vec3(0.0f, 0.0f, 2.5f));
    scene.addObject(object1);

    auto object2 = std::make_shared<cg::Container>(*object1.get());
    object2->Translate(cg::vec3(-2.0f, 0.0f, 1.0f));
    object2->Rotate(0.0f, 90.0f, 0.0f);

    auto object3 = std::make_shared<cg::Container>(*object1.get());
    object3->Translate(cg::vec3(2.5f, 0.5f, 1.0f));
    object3->Rotate(0.0f, 45.0f, 45.0f);

    scene.addObject(object2);
    scene.addObject(object3);

    auto pointLight1 = std::make_shared<cg::PointLight>(cg::Color(0.28f, 1.0f, 1.0f, 1.0f), 7.0f);
    pointLight1->Translate(cg::vec3(-2.0f, 1.0f, 0.0f));
    scene.addLight(pointLight1);

    auto pointLight2 = std::make_shared<cg::PointLight>(cg::Color(0.7f, 0.0f, 0.0f, 1.0f), 7.0f);
    pointLight2->Translate(cg::vec3(2.0f, -1.0f, 0.0f));
    scene.addLight(pointLight2);

    auto ambient = std::make_shared<cg::AmbientLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.2f);
    scene.addLight(ambient);
    return scene;
}

// --- HIER IST DIE KORRIGIERTE DNA SZENE ---

cg::Scene createOwnScene()
{
    cg::Scene scene("DNA Double Helix Fixed");

    // --- SETUP ---
    const int num_segments = 50;        // Länge der DNA
    const float helix_radius = 2.0f;    // Radius (Breite)
    const float height_step = 0.35f;    // Vertikaler Abstand pro Stufe
    const float angle_step = 25.0f;     // Drehung pro Stufe in Grad
    const float cube_size = 0.3f;       // Größe der "Atome"

    // Farben definieren
    cg::Color col_strand_a = cg::blue();
    cg::Color col_strand_b = cg::Color(1.0f, 0.0f, 0.0f, 1.0f); // Rot
    cg::Color col_bridge   = cg::Color(0.7f, 0.7f, 0.7f, 1.0f); // Grau (WICHTIG: 4 Werte inkl. Alpha!)

    for (int i = 0; i < num_segments; ++i)
    {
        // 1. Berechnung der Basiswerte
        float angle_deg = i * angle_step;
        float angle_rad = glm::radians(angle_deg); // Umrechnung in Bogenmaß für sin/cos

        float y = -8.0f + (i * height_step); // Startet weit unten und geht nach oben

        // 2. Berechnung der exakten Kreisposition mittels Trigonometrie
        // Das verhindert den "Turm-Effekt", da wir die Position absolut setzen.
        float x_pos = helix_radius * std::cos(angle_rad);
        float z_pos = helix_radius * std::sin(angle_rad);

        // Strang A Position
        cg::vec3 pos_a(x_pos, y, z_pos);

        // Strang B Position (genau gegenüberliegend -> einfach negieren)
        cg::vec3 pos_b(-x_pos, y, -z_pos);

        // --- STRANG A (Blau) ---
        auto atom_a = std::make_shared<cg::Cube>(cube_size, col_strand_a);
        atom_a->Translate(pos_a);
        // Optional: Den Würfel selbst mitdrehen, damit er der Kurve folgt
        atom_a->Rotate(0.0f, -angle_deg, 0.0f);
        scene.addObject(atom_a);

        // --- STRANG B (Rot) ---
        auto atom_b = std::make_shared<cg::Cube>(cube_size, col_strand_b);
        atom_b->Translate(pos_b);
        atom_b->Rotate(0.0f, -angle_deg, 0.0f);
        scene.addObject(atom_b);

        // --- SPROSSEN (Verbindungslinie) ---
        // Wir erzeugen kleine Würfel auf der direkten Linie zwischen A und B
        int bridge_steps = 6; // Anzahl der Teile pro Sprosse
        for(int k = 1; k < bridge_steps; k++)
        {
            float t = (float)k / (float)bridge_steps; // Interpolationsfaktor (0.0 bis 1.0)

            // Lineare Interpolation (LERP) für die Position
            cg::vec3 pos_bridge = pos_a + t * (pos_b - pos_a);

            auto bridge_part = std::make_shared<cg::Cube>(0.15f, col_bridge);
            bridge_part->Translate(pos_bridge);
            bridge_part->Rotate(0.0f, -angle_deg, 0.0f); // Auch mitdrehen
            scene.addObject(bridge_part);
        }
    }

    // --- LICHTER ---
    auto pointLight = std::make_shared<cg::PointLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 20.0f);
    pointLight->Translate(cg::vec3(5.0f, 5.0f, -5.0f));
    scene.addLight(pointLight);

    auto ambientLight = std::make_shared<cg::AmbientLight>(cg::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.3f);
    scene.addLight(ambientLight);

    return scene;
}

int main(const int argc, const char** argv)
{
    std::cout << "Uni Stuttgart - CG Exercise" << std::endl;

    // Create different scenes
    std::vector<cg::Scene> scenes;
    scenes.push_back(createTriangle());
    scenes.push_back(createCube());
    scenes.push_back(createSphere());
    scenes.push_back(createComplexScene());
    scenes.push_back(createOwnScene());

    // Create rasterizer
    cg::Rasterizer rasterizer(cg::defaultCamera(), scenes, cg::Rasterizer::FILLED, 1600, 900);

    // Set the active scene to the DNA Double Helix (index 4)
    rasterizer.accessActiveScene() = 4;

    // Adjust camera position (Further back to see the whole helix)
    rasterizer.accessCamera().setPosition(cg::Point3D(0.0f, 0.0f, -18.0f));
    rasterizer.accessCamera().setLookat(cg::Point3D(0.0f, 0.0f, 0.0f));

    // Start image viewer
    cg::ImageViewer image_viewer(rasterizer);
    image_viewer.run();

    return 0;
}