#pragma once

#include "ImageTraits.h"
#include "ImageBase.h"

#include <array>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>

namespace cg
{
    /// <summary>
    /// Image class
    /// </summary>
    /// <tparam name="color_space">Color space (RGB, HSV, ...)</tparam>
    template <color_space_t color_space = color_space_t::RGB>
    class image : public image_base
    {
    public:
        /// Integer or floating point type for representing the color values
        using value_type = float;

        /// Tuple type for storing all color channels of a pixel
        using tuple_type = std::array<value_type, color_channels<color_space>::value>;

        /// Data type for containing all pixels of the image
        using data_type = std::vector<tuple_type>;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="width">Image width</param>
        /// <param name="height">Image height</param>
        image(unsigned int width, unsigned int height);

        /// <summary>
        /// Copy constructor
        /// </summary>
        /// <param name="other">Image to copy</param>
        image(const image& other);

        /// <summary>
        /// Copy assignment operator
        /// </summary>
        /// <param name="rhs">Image to copy-assign from</param>
        image& operator=(const image& rhs);

        /// <summary>
        /// Get color space
        /// </summary>
        /// <returns>Color space used (RGB, HSV, ...)</returns>
        color_space_t get_color_space() const;

        /// <summary>
        /// Initialize image with zero-values
        /// </summary>
        virtual void initialize();

        /// <summary>
        /// Initialize image with given value
        /// </summary>
        /// <param name="initial_value">Initial value</param>
        void initialize(value_type initial_value);

        /// <summary>
        /// Initialize image with given tuple
        /// </summary>
        /// <param name="initial_value">Initial value</param>
        void initialize(const tuple_type& initial_value);

        tuple_type* data() noexcept { return m_data.data(); }
        const tuple_type* data() const noexcept { return m_data.data(); }

        /// <summary>
        /// Access pixel
        /// </summary>
        /// <param name="i">Index in x direction</param>
        /// <param name="j">Index in y direction</param>
        /// <returns>Pixel value</returns>
        const tuple_type& at(unsigned int i, unsigned int j) const;
        tuple_type& at(unsigned int i, unsigned int j);

        const tuple_type& operator()(unsigned int i, unsigned int j) const;
        tuple_type& operator()(unsigned int i, unsigned int j);

    private:
        /// <summary>
        /// Calculate the pixel index
        /// </summary>
        /// <param name="i">Index in x direction</param>
        /// <param name="j">Index in y direction</param>
        /// <returns>Pixel index</returns>
        unsigned int index(const unsigned int i, const unsigned int j) const;

        /// Image data
        data_type m_data;
    };
}

template <cg::color_space_t color_space>
inline cg::image<color_space>::image(const unsigned int width, const unsigned int height)
    : cg::image_base(width, height)
{
    this->m_data.resize(width * height);
}

template <cg::color_space_t color_space>
inline cg::image<color_space>::image(const image& other)
    : cg::image_base(other.width, other.height), m_data(other.m_data)
{
}

template <cg::color_space_t color_space>
inline cg::image<color_space>& cg::image<color_space>::operator=(const image& rhs)
{
    if (this != &rhs) {
        image tmp(rhs);
        std::swap(m_data, tmp.m_data);
        std::swap(width, tmp.width);
        std::swap(height, tmp.height);
    }

    return *this;
}

template <cg::color_space_t color_space>
inline cg::color_space_t cg::image<color_space>::get_color_space() const
{
    return color_space;
}

template <cg::color_space_t color_space>
inline void cg::image<color_space>::initialize()
{
    initialize(static_cast<value_type>(0));
}

template <cg::color_space_t color_space>
inline void cg::image<color_space>::initialize(const value_type initial_value)
{
    tuple_type tuple;

    for (auto& value : tuple)
    {
        value = initial_value;
    }

    initialize(tuple);
}

template <cg::color_space_t color_space>
inline void cg::image<color_space>::initialize(const tuple_type& initial_value)
{
    for (auto& tuple : this->m_data)
    {
        tuple = initial_value;
    }
}

template <cg::color_space_t color_space>
inline const typename cg::image<color_space>::tuple_type& cg::image<color_space>::at(const unsigned int i, const unsigned int j) const
{
    return this->m_data[index(i, j)];
}

template <cg::color_space_t color_space>
inline typename cg::image<color_space>::tuple_type& cg::image<color_space>::at(const unsigned int i, const unsigned int j)
{
    return this->m_data[index(i, j)];
}

template <cg::color_space_t color_space>
inline const typename cg::image<color_space>::tuple_type& cg::image<color_space>::operator()(const unsigned int i, const unsigned int j) const
{
    return at(i, j);
}

template <cg::color_space_t color_space>
inline typename cg::image<color_space>::tuple_type& cg::image<color_space>::operator()(const unsigned int i, const unsigned int j)
{
    return at(i, j);
}

template <cg::color_space_t color_space>
inline unsigned int cg::image<color_space>::index(const unsigned int i, const unsigned int j) const
{
    if (i >= this->width || j >= this->height)
    {
        throw std::runtime_error("Illegal pixel");
    }

    return i + j * this->width;
}