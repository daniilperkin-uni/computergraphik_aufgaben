#include "PointLight.h"

#include <string>

cg::PointLight::PointLight(const Color& color, const float intensity) : LightObject(color, intensity, false)
{ }

std::string cg::PointLight::getShapeName() const
{
    return "Point Light";
}

cg::ColorInfo cg::PointLight::getColor(const Point3D& obj) const
{
    const auto position = Point3D(Object::transformation * vec4(0.0f, 0.0f, 0.0f, 1.0f));

    return ColorInfo{ LightObject::color, LightObject::intensity, LightObject::ambient, obj - position };
}
