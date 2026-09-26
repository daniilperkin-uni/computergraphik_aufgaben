#ifndef ImageFilter_hpp
#define ImageFilter_hpp

#include <algorithm>
#include <exception>
#include <utility>
#include <cmath>

#include "Image.h"

namespace cg
{
    namespace filter
    {
        enum BorderPolicy { CLAMP_TO_EDGE, MIRROR, REPEAT };

        // use empty namespace for "private" functions
        namespace
        {
            /**
             * Applies offset to image coordinates.
             * Guarantees to return valid coordinates even if the offset is
             * theoretically out of bounds.
             *
             * @param image Input image - defines the valid image region.
             * @param coordinates Input coordinates within the valid image region.
             * @param offset Offset values that are applied to the input coordinates.
             * @param border_policy Policy for handling out of bounds coordinates.
             */
            template <cg::color_space_t color_space>
            std::pair<unsigned int, unsigned int> offsetImageCoordinates(
                image<color_space> const& image,
                std::pair<unsigned int, unsigned int> coordinates,
                std::pair<int, int> offset,
                BorderPolicy border_policy)
            {
                const int x = static_cast<int>(std::get<0>(coordinates)) + std::get<0>(offset);
                const int y = static_cast<int>(std::get<1>(coordinates)) + std::get<1>(offset);

                if (x < 0 || x >= static_cast<int>(image.get_width()) ||
                    y < 0 || y >= static_cast<int>(image.get_height()))
                {
                    switch (border_policy)
                    {
                    case cg::filter::CLAMP_TO_EDGE:
                        return std::make_pair(
                            std::min(std::max(static_cast<int>(std::get<0>(coordinates)) + std::get<0>(offset), 0), static_cast<int>(image.get_width() - 1)),
                            std::min(std::max(static_cast<int>(std::get<1>(coordinates)) + std::get<1>(offset), 0), static_cast<int>(image.get_height() - 1))
                        );

                        break;

                            //Aufgabe 3.6
                        case cg::filter::MIRROR:
                            // Out-of-bounds durch Spiegelung behandeln

                            // Links/Oben spiegeln
                            if (x < 0) x = -x - 1;
                            // Rechts/Unten spiegeln
                            else if (x >= static_cast<int>(image.get_width())) x = 2 * static_cast<int>(image.get_width()) - 1 - x;

                            if (y < 0) y = -y - 1;
                            else if (y >= static_cast<int>(image.get_height())) y = 2 * static_cast<int>(image.get_height()) - 1 - y;

                            // Safety Clamp, falls Koordinaten immer noch außerhalb
                            x = std::min(std::max(x, 0), static_cast<int>(image.get_width() - 1));
                            y = std::min(std::max(y, 0), static_cast<int>(image.get_height() - 1));

                            return{ x,y };

                            break;
                            //Aufgabe 3.6
                        case cg::filter::REPEAT:
                        {
                            // Wrap-around (Tiling)
                            int w = static_cast<int>(image.get_width());
                            int h = static_cast<int>(image.get_height());

                            // Modulo mit Korrektur für negative Werte
                            x = (x % w + w) % w;
                            y = (y % h + h) % h;
                        }
                            //Ende



                        return{ x,y };

                        break;

                    default:
                        throw std::exception();

                        break;
                    }
                }
                else
                {
                    return std::make_pair(
                        std::get<0>(coordinates) + std::get<0>(offset),
                        std::get<1>(coordinates) + std::get<1>(offset)
                    );
                }
            }
        }

        /**
         * Basic Filter-Kernel class.
         */
        class Kernel
        {
        public:
            /**
             * Construct a new kernel with given extents in x and y direction.
             * Note that the overal size of the kernel is (2 * extent_X +1) x (2 * extent_Y +1),
             * i.e. a kernel of extent {2,2} has a size of 5x5.
             */
            Kernel(std::pair<unsigned int, unsigned int> extents);

            /**
             * Get a single entry of the filter kernel.
             * Coordinates are given relative to the center of the kernel.
             */
            float getValue(int x, int y) const;

            float* data();

            float const* data() const;

            /**
             * Returns the min and max horizontal coordinate within the kernel.
             * Coordinates are given relative to the center of the kernel,
             * i.e. -2 and 2 for a kernel with a horizontal extent of 2.
             */
            std::pair<int, int> getHorizontalRange() const;
            /**
            * Returns the min and max vertical coordinate within the kernel.
            * Coordinates are given relative to the center of the kernel,
            * i.e. -2 and 2 for a kernel with a vertical extent of 2.
            */
            std::pair<int, int> getVerticalRange() const;

            /**
             * Sets the value of the kernel for a given position.
             * Note that the position is given relative to the center of the kernel.
             */
            void setValue(int x, int y, float v);

        private:
            std::vector<float> m_data;
            std::pair<unsigned int, unsigned int> m_extents;

            /**
            * Compute actual data index for a position given relative to the center of the kernel.
            */
            size_t getDataIndex(int x, int y) const;
        };

        /** Create 2D gaussian filter kernel. */
        Kernel build2DGaussianKernel(std::pair<unsigned int, unsigned int> extents, float sigma);

        /** Create 1D horizontal filter kernel for a seperated gaussian filter. */
        Kernel build1DHorizontalGaussianKernel(unsigned int extent, float sigma);

        /** Create 1D vertical filter kernel for a seperated gaussian filter. */
        Kernel build1DVerticalGaussianKernel(unsigned int extent, float sigma);

        /** Create a simple 3x3 edge detection filter kernel. */
        Kernel buildEdgeDetectionKernel();

        /** Apply a filter kernel to an image. */
        template <color_space_t color_space>
        image<color_space> filterImage(
            image<color_space> const& original,
            Kernel const& filter_kernel,
            BorderPolicy border_policy = CLAMP_TO_EDGE);

        /** Computes a channelwise absolute value version of a given image.*/
        template <cg::color_space_t color_space>
        cg::image<color_space> absValueImage(
            image<color_space> const& original);

        /** Compute an image blurred with a 2D gaussian filter. */
        template <cg::color_space_t color_space>
        cg::image<color_space> gaussian2D(
            image<color_space> const& original,
            std::pair<int, int> kernelExtents,
            float sigma,
            BorderPolicy borders = CLAMP_TO_EDGE);

        /** Compute an image blurred with a seperated 2D gaussian filter. */
        template <cg::color_space_t color_space>
        cg::image<color_space> seperatedGaussian2D(
            image<color_space> const& original,
            std::pair<int, int> kernelExtents,
            float sigma,
            BorderPolicy borders = CLAMP_TO_EDGE);

        /** Computes an image with extracted edges. */
        template <cg::color_space_t color_space>
        cg::image<color_space> edgeDetection2D(
            image<color_space> const& original,
            BorderPolicy borders = CLAMP_TO_EDGE);
    }
}

template <cg::color_space_t color_space>
cg::image<color_space> cg::filter::filterImage(image<color_space> const& original, Kernel const& filter_kernel, BorderPolicy border_policy)
{
    cg::image<color_space> filtered(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            typename cg::image<color_space>::tuple_type pixel_value;
            for (auto& channel : pixel_value)
                channel = 0;



            //Aufgabe 3.1

            // Kernel-Bereiche (relative Coords) holen
            auto h_range = filter_kernel.getHorizontalRange();
            auto v_range = filter_kernel.getVerticalRange();

            // Loop über Kernel-Höhe
            for (int ky = v_range.first; ky <= v_range.second; ++ky)
            {
                // ... und Breite
                for (int kx = h_range.first; kx <= h_range.second; ++kx)
                {
                    // Gewicht holen
                    float weight = filter_kernel.getValue(kx, ky);

                    // Sample-Position im Originalbild berechnen
                    auto sample_pos = offsetImageCoordinates(original, {i, j}, {kx, ky}, border_policy);

                    // Pixelwert lesen
                    auto const& sample_val = original(sample_pos.first, sample_pos.second);

                    // Gewichtete Werte pro Kanal aufaddieren
                    for (size_t c = 0; c < pixel_value.size(); ++c)
                    {
                        pixel_value[c] += weight * sample_val[c];
                    }
                }
            }

            //Ende

            filtered(i, j) = pixel_value;
        }
    }

    return filtered;
}

template <cg::color_space_t color_space>
cg::image<color_space> cg::filter::absValueImage(image<color_space> const& original)
{
    cg::image<color_space> filtered(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            typename cg::image<color_space>::tuple_type pixel_value;
            typename cg::image<color_space>::tuple_type orig_pixel_value =
                original(i, j);

            for (int c = 0; c < color_channels<color_space>::value; ++c)
                pixel_value[c] = std::fabs(orig_pixel_value[c]);

            filtered(i, j) = pixel_value;
        }
    }

    return filtered;
}

template <cg::color_space_t color_space>
cg::image<color_space> cg::filter::gaussian2D(
    image<color_space> const& original,
    std::pair<int, int> kernelExtents,
    float sigma,
    BorderPolicy borders)
{

    //Aufgabe 3.5
    
    // Extents auf unsigned casten
    std::pair<unsigned int, unsigned int> extents(
        static_cast<unsigned int>(kernelExtents.first),
        static_cast<unsigned int>(kernelExtents.second)
    );

    // 2D-Kernel bauen
    Kernel kernel = build2DGaussianKernel(extents, sigma);

    // Filter anwenden
    return filterImage(original, kernel, borders);
}

template <cg::color_space_t color_space>
cg::image<color_space> cg::filter::seperatedGaussian2D(
    image<color_space> const& original,
    std::pair<int, int> kernelExtents,
    float sigma,
    BorderPolicy borders)
{
    //Aufgabe 3.5

    // Extents casten
    unsigned int extX = static_cast<unsigned int>(kernelExtents.first);
    unsigned int extY = static_cast<unsigned int>(kernelExtents.second);

    // 1D-Kernel erstellen (separabel = effizienter)
    Kernel kH = build1DHorizontalGaussianKernel(extX, sigma);
    Kernel kV = build1DVerticalGaussianKernel(extY, sigma);

    // Erst horizontal filtern
    auto imgH = filterImage(original, kH, borders);

    // dann vertikal auf das Ergebnis anwenden
    return filterImage(imgH, kV, borders);

    //Ende
}

template <cg::color_space_t color_space>
cg::image<color_space> cg::filter::edgeDetection2D(
    image<color_space> const& original, BorderPolicy borders)


//aufgabe 3.3
{
    // Edge-Kernel erstellen
    Kernel kernel = buildEdgeDetectionKernel();

    // Filter anwenden (Faltung)
    image<color_space> filtered = filterImage(original, kernel, borders);

    // Betrag bilden
    return absValueImage(filtered);
}
//ende


#endif // !ImageFilter_hpp
