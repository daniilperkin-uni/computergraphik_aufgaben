#include "Object.h"

#include "glm/gtc/matrix_transform.hpp"

cg::Object::Object() : transformation(unitMat4()), visible(true)
{ }

cg::Object::Object(const mat4& transformation) : transformation(transformation), visible(true)
{ }

void cg::Object::Transform(const mat4& transformation)
{
    this->transformation *= transformation;
}

void cg::Object::Rotate(float alpha, float beta, float gamma)
{
    this->transformation = glm::rotate(this->transformation, alpha, vec3(1.0f, 0.0f, 0.0f));
    this->transformation = glm::rotate(this->transformation, beta, vec3(0.0f, 1.0f, 0.0f));
    this->transformation = glm::rotate(this->transformation, gamma, vec3(0.0f, 0.0f, 1.0f));
}

void cg::Object::Translate(const vec3& translation)
{
    this->transformation = glm::translate(this->transformation, vec3(-translation[0], translation[1], translation[2]));
}

void cg::Object::Scale(const vec3& scale)
{
    this->transformation = glm::scale(this->transformation, scale);
}

cg::mat4 cg::Object::getTransformation() const
{
    return this->transformation;
}

bool& cg::Object::accessVisibility()
{
    return this->visible;
}

bool cg::Object::isVisible() const
{
    return this->visible;
}
