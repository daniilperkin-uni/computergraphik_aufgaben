#include "AmbientLight.h"

#include <string>

cg::AmbientLight::AmbientLight(const Color& color, const float intensity) : LightObject(color, intensity, true)
{ }

std::string cg::AmbientLight::getShapeName() const
{
    return "Ambient Light";
}

cg::ColorInfo cg::AmbientLight::getColor(const Point3D&) const
{
    return ColorInfo{ LightObject::color, LightObject::intensity, LightObject::ambient, zeroVec3() };
}
