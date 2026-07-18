#include "ImageConverter.h"

#include <cmath>

cg::image<cg::color_space_t::HSV> cg::image_converter::rgb_to_hsv(const image<color_space_t::RGB>& original)
{
    // Convert RGB to HSV
    cg::image<cg::color_space_t::HSV> converted(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            const float r = original(i, j)[0];
            const float g = original(i, j)[1];
            const float b = original(i, j)[2];

            const float c_max = std::max(std::max(r, g), b);
            const float c_min = std::min(std::min(r, g), b);
            const float delta = c_max - c_min;

            float h;

            if (delta == 0.0f)
            {
                h = 0.0f;
            }
            else if (c_max == r)
            {
                auto remainder = static_cast<float>(std::fmod((g - b) / delta, 6));
                remainder += (remainder < 0.0f) ? 6.0f : 0.0f;

                h = remainder / 6.0f;
            }
            else if (c_max == g)
            {
                h = ((b - r) / delta + 2.0f) / 6.0f;
            }
            else
            {
                h = ((r - g) / delta + 4.0f) / 6.0f;
            }

            const float s = (c_max == 0.0f) ? 0.0f : delta / c_max;
            const float v = c_max;

            converted(i, j)[0] = h;
            converted(i, j)[1] = s;
            converted(i, j)[2] = v;
        }
    }

    return converted;
}

cg::image<cg::color_space_t::RGB> cg::image_converter::hsv_to_rgb(const image<color_space_t::HSV>& original)
{
    // Convert HSV to RGB
    image<color_space_t::RGB> converted(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            const float h = original(i, j)[0];
            const float s = original(i, j)[1];
            const float v = original(i, j)[2];

            const float h6 = h * 6.0f;

            const float c = s * v;
            const float x = c * (1.0f - std::abs(static_cast<float>(std::fmod(h6, 2)) - 1.0f));
            const float m = v - c;

            float r, g, b;

            if (h6 < 1.0f)
            {
                r = c;
                g = x;
                b = 0.0f;
            }
            else if (h6 < 2.0f)
            {
                r = x;
                g = c;
                b = 0.0f;
            }
            else if (h6 < 3.0f)
            {
                r = 0.0f;
                g = c;
                b = x;
            }
            else if (h6 < 4.0f)
            {
                r = 0.0f;
                g = x;
                b = c;
            }
            else if (h6 < 5.0f)
            {
                r = x;
                g = 0.0f;
                b = c;
            }
            else
            {
                r = c;
                g = 0.0f;
                b = x;
            }

            r += m;
            g += m;
            b += m;

            converted(i, j)[0] = r;
            converted(i, j)[1] = g;
            converted(i, j)[2] = b;
        }
    }

    return converted;
}

cg::image<cg::color_space_t::Gray> cg::image_converter::rgb_to_gray(const image<color_space_t::RGB>& original)
{
    // Convert RGB to grayscale
    image<color_space_t::Gray> converted(original.get_width(), original.get_height());

    const float r_weight = 0.2989f;
    const float g_weight = 0.5870f;
    const float b_weight = 0.1140f;

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            converted(i, j)[0] = r_weight * original(i, j)[0] + g_weight * original(i, j)[1] + b_weight * original(i, j)[2];
        }
    }

    return converted;
}

cg::image<cg::color_space_t::BW> cg::image_converter::gray_to_bw(const image<color_space_t::Gray>& original)
{
    // Convert grayscale to black and white
    image<color_space_t::BW> converted(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            converted(i, j)[0] = (original(i, j)[0] >= 0.5f) ? 1.0f : 0.0f;
        }
    }

    return converted;
}