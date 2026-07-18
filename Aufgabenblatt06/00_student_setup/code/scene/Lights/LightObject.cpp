#include "LightObject.h"

cg::LightObject::LightObject(const Color& color, const float intensity, const bool ambient) : color(color), intensity(intensity), ambient(ambient)
{ }

cg::Color& cg::LightObject::accessColor()
{
    return this->color;
}

float& cg::LightObject::accessIntensity()
{
    return this->intensity;
}
