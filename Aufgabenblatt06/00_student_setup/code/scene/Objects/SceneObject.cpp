#include "SceneObject.h"

cg::SceneObject::SceneObject(const Color& color) : color(color)
{ }

cg::Color& cg::SceneObject::accessColor()
{
    return this->color;
}

cg::Color cg::SceneObject::getColor() const
{
    return this->color;
}
