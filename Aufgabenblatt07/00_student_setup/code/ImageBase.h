#pragma once

#include "ImageTraits.h"

namespace cg
{
    /// <summary>
    /// Image base class
    /// </summary>
    class image_base
    {
    public:
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="width">Image width</param>
        /// <param name="height">Image height</param>
        image_base(unsigned int width, unsigned int height);

        /// <summary>
        /// Get width or height
        /// </summary>
        /// <returns>Width / height</returns>
        unsigned int get_width() const;
        unsigned int get_height() const;

        /// <summary>
        /// Initialize image to zero-values
        /// </summary>
        virtual void initialize() = 0;

    protected:
        /// Width and height
        unsigned int width;
        unsigned int height;
    };
}