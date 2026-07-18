#include <cmath>

#include "ImageFilter.h"

namespace cg
{
    namespace filter
    {
        namespace
        {
            /** Computes and stores gaussian values in given kernel for the given sigma value */
            void setGaussianValues(Kernel& k, float sigma)
            {
                float sum = 0.0f;
                float twoSigmaSq = 2.0f * sigma * sigma;



                //Aufgabe 3.4

                // Kernel-Ranges holen
                auto h_range = k.getHorizontalRange();
                auto v_range = k.getVerticalRange();

                // Loop über Kernel
                for (int y = v_range.first; y <= v_range.second; ++y)
                {
                    for (int x = h_range.first; x <= h_range.second; ++x)
                    {
                        // Gauss-Wert berechnen (Vorfaktor egal, wird später normalisiert)
                        float val = std::exp(-(static_cast<float>(x * x + y * y)) / twoSigmaSq);
                        k.setValue(x, y, val);

                        // Summe tracken
                        sum += val;
                    }
                }

                // Normalisieren (Summe muss 1 sein)
                if (sum != 0.0f)
                {
                    for (int y = v_range.first; y <= v_range.second; ++y)
                    {
                        for (int x = h_range.first; x <= h_range.second; ++x)
                        {
                            k.setValue(x, y, k.getValue(x, y) / sum);
                        }
                    }
                }
            }
        }
        //Ende

        Kernel::Kernel(std::pair<unsigned int, unsigned int> extents)
            : m_data((std::get<0>(extents) * 2 + 1)* (std::get<1>(extents) * 2 + 1)), m_extents(extents)
        {
            std::fill(m_data.begin(), m_data.end(), 0.0f);
            setValue(0, 0, 1.0f);
        }

        float Kernel::getValue(int x, int y) const
        {
            return m_data[getDataIndex(x, y)];
        }

        float* Kernel::data()
        {
            return m_data.data();
        }

        float const* Kernel::data() const
        {
            return m_data.data();
        }

        std::pair<int, int> Kernel::getHorizontalRange() const
        {
            return std::make_pair(-static_cast<int>(std::get<0>(m_extents)), static_cast<int>(std::get<0>(m_extents)));
        }

        std::pair<int, int> Kernel::getVerticalRange() const
        {
            return std::make_pair(-static_cast<int>(std::get<1>(m_extents)), static_cast<int>(std::get<1>(m_extents)));
        }

        void Kernel::setValue(int x, int y, float v)
        {
            m_data[getDataIndex(x, y)] = v;
        }

        size_t Kernel::getDataIndex(int x, int y) const
        {
            x += std::get<0>(m_extents);
            y += std::get<1>(m_extents);

            return y * (std::get<0>(m_extents) * 2 + 1) + x;
        }


        Kernel build2DGaussianKernel(std::pair<unsigned int, unsigned int> extents, float sigma)
        {
            Kernel k(extents);

            setGaussianValues(k, sigma);

            return k;
        }

        Kernel build1DHorizontalGaussianKernel(unsigned int extent, float sigma)
        {
            Kernel k(std::make_pair(extent, 0));

            setGaussianValues(k, sigma);

            return k;
        }

        Kernel build1DVerticalGaussianKernel(unsigned int extent, float sigma)
        {
            Kernel k(std::make_pair(0, extent));

            setGaussianValues(k, sigma);

            return k;
        }

        Kernel buildEdgeDetectionKernel()
        {
            Kernel k(std::make_pair(1, 1));

            //aufgabe 3.2

            // 3x3 Laplace-Kernel für Kanten (Summe = 0)

            // Oben
            k.setValue(-1, -1, -1.0f); k.setValue( 0, -1, -1.0f); k.setValue( 1, -1, -1.0f);
            // Mitte (Zentrum 8, Rest -1)
            k.setValue(-1,  0, -1.0f); k.setValue( 0,  0,  8.0f); k.setValue( 1,  0, -1.0f);
            // Unten
            k.setValue(-1,  1, -1.0f); k.setValue( 0,  1, -1.0f); k.setValue( 1,  1, -1.0f);

            //Ende


            return k;
        }
    }
}
