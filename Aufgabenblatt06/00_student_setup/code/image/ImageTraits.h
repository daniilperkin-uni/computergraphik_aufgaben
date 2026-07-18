#pragma once

namespace cg
{
    /// Color space
    enum class color_space_t
    {
        BW, Gray, RGB, HSV, RGBA
    };

    template <color_space_t color_space>
    struct color_channels
    {
        static constexpr unsigned int value = 3;
    };

    template <>
    struct color_channels<color_space_t::Gray>
    {
        static constexpr unsigned int value = 1;
    };

    template <>
    struct color_channels<color_space_t::BW>
    {
        static constexpr unsigned int value = 1;
    };

    template <>
    struct color_channels<color_space_t::RGB>
    {
        static constexpr unsigned int value = 3;
    };

    template <>
    struct color_channels<color_space_t::RGBA>
    {
        static constexpr unsigned int value = 4;
    };
}
