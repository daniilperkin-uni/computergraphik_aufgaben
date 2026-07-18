#pragma once

#include "Image.h"

namespace cg
{
    /// <summary>
    /// Class for converting images
    /// </summary>
    class image_converter
    {
    public:
        /// <summary>
        /// Convert image from RGB to HSV
        /// </summary>
        /// <param name="original">Original image</param>
        /// <returns>Converted image</returns>
        static image<color_space_t::HSV> rgb_to_hsv(const image<color_space_t::RGB>& original);

        /// <summary>
        /// Convert image from HSV to RGB
        /// </summary>
        /// <param name="original">Original image</param>
        /// <returns>Converted image</returns>
        static image<color_space_t::RGB> hsv_to_rgb(const image<color_space_t::HSV>& original);

        /// <summary>
        /// Convert image from RGB to grayscale
        /// </summary>
        /// <param name="original">Original image</param>
        /// <returns>Converted image</returns>
        static image<color_space_t::Gray> rgb_to_gray(const image<color_space_t::RGB>& original);

        /// <summary>
        /// Convert image from grayscale to black and white
        /// </summary>
        /// <param name="original">Original image</param>
        /// <returns>Converted image</returns>
        static image<color_space_t::BW> gray_to_bw(const image<color_space_t::Gray>& original);
    };
}