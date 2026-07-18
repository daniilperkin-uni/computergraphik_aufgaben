#include "ImageBase.h"

cg::image_base::image_base(const unsigned int width, const unsigned int height) : width(width), height(height)
{
}

unsigned int cg::image_base::get_width() const
{
    return this->width;
}

unsigned int cg::image_base::get_height() const
{
    return this->height;
}

void cg::image_base::resize(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;

    resizeImage();
}
