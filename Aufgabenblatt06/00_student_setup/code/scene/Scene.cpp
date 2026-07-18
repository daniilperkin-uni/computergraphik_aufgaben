#include "Scene.h"

#include <memory>
#include <string>
#include <vector>

cg::Scene::Scene(const std::string& name) : name(name)
{ }

void cg::Scene::addObject(std::shared_ptr<SceneObject> object)
{
    this->objects.push_back(object);
}

std::vector<std::shared_ptr<cg::SceneObject>> cg::Scene::getObjects()
{
    return this->objects;
}

void cg::Scene::addLight(std::shared_ptr<LightObject> light)
{
    this->lights.push_back(light);
}

std::vector<std::shared_ptr<cg::LightObject>> cg::Scene::getLights()
{
    return this->lights;
}

const std::string& cg::Scene::getName() const
{
    return this->name;
}
