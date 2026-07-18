#include "ImageConverter.hpp"

#include <cmath>

cg::image<cg::color_space_t::HSV> cg::image_converter::rgb_to_hsv(const image<color_space_t::RGB>& original)
{
    // Convert RGB to HSV
    cg::image<cg::color_space_t::HSV> converted(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            const float red_component = original(i, j)[0];
            const float green_component = original(i, j)[1];
            const float blue_component = original(i, j)[2];

            const float max_color_value = std::max(std::max(red_component, green_component), blue_component);
            const float min_color_value = std::min(std::min(red_component, green_component), blue_component);
            const float color_difference = max_color_value - min_color_value;

            float hue = 0.f, saturation = 0.f, value = 0.f;

            // Value (V) ist der maximale Kanalwert
            value = max_color_value;

            // Wenn color_difference 0 ist, ist die Farbe ein Grauton. Sättigung ist 0, Farbton ist undefiniert (wir setzen 0).
            if (color_difference == 0) {
                saturation = 0;
                hue = 0;
            } else {
                // Saturation (S) ist color_difference / max_color_value
                saturation = color_difference / max_color_value;

                // Hue (H) berechnen
                if (max_color_value == red_component) {
                    hue = 60.f * (fmodf((green_component - blue_component) / color_difference, 6.f));
                } else if (max_color_value == green_component) {
                    hue = 60.f * (((blue_component - red_component) / color_difference) + 2.f);
                } else { // max_color_value == blue_component
                    hue = 60.f * (((red_component - green_component) / color_difference) + 4.f);
                }

                // Sicherstellen, dass hue im Bereich [0, 360) liegt
                if (hue < 0) {
                    hue += 360.f;
                }
            }

            // Den Hue-Wert auf den Bereich [0, 1] normalisieren, wie in der Aufgabe gefordert.
            converted(i, j)[0] = hue / 360.0f;
            converted(i, j)[1] = saturation;
            converted(i, j)[2] = value;
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
            float hue = original(i, j)[0];
            const float saturation = original(i, j)[1];
            const float value = original(i, j)[2];

            float red_component = 0.f, green_component = 0.f, blue_component = 0.f;

            // Wenn Sättigung 0 ist, ist die Farbe ein Grauton.
            if (saturation == 0) {
                red_component = green_component = blue_component = value;
            } else {
                // Wir berechnen hue_sector aus dem normalisierten hue.
                // Der Bereich [0, 1] entspricht 6 Sektoren à 60°.
                const float hue_sector = hue * 6.0f;

                const float chroma = value * saturation; // Chroma
                const float intermediate_value = chroma * (1.f - fabsf(fmodf(hue_sector, 2.f) - 1.f));
                const float brightness_adjustment = value - chroma;

                float temp_red = 0, temp_green = 0, temp_blue = 0;

                if (hue_sector >= 0 && hue_sector < 1) {
                    temp_red = chroma; temp_green = intermediate_value; temp_blue = 0;
                } else if (hue_sector >= 1 && hue_sector < 2) {
                    temp_red = intermediate_value; temp_green = chroma; temp_blue = 0;
                } else if (hue_sector >= 2 && hue_sector < 3) {
                    temp_red = 0; temp_green = chroma; temp_blue = intermediate_value;
                } else if (hue_sector >= 3 && hue_sector < 4) {
                    temp_red = 0; temp_green = intermediate_value; temp_blue = chroma;
                } else if (hue_sector >= 4 && hue_sector < 5) {
                    temp_red = intermediate_value; temp_green = 0; temp_blue = chroma;
                } else { // hue_sector >= 5 && hue_sector < 6
                    temp_red = chroma; temp_green = 0; temp_blue = intermediate_value;
                }

                red_component = temp_red + brightness_adjustment;
                green_component = temp_green + brightness_adjustment;
                blue_component = temp_blue + brightness_adjustment;
            }

            converted(i, j)[0] = red_component;
            converted(i, j)[1] = green_component;
            converted(i, j)[2] = blue_component;
        }
    }

    return converted;
}

cg::image<cg::color_space_t::Gray> cg::image_converter::rgb_to_gray(const image<color_space_t::RGB>& original)
{
    // Convert RGB to grayscale
    image<color_space_t::Gray> converted(original.get_width(), original.get_height());

    for (unsigned int j = 0; j < original.get_height(); ++j)
    {
        for (unsigned int i = 0; i < original.get_width(); ++i)
        {
            const float red_component = original(i, j)[0];
            const float green_component = original(i, j)[1];
            const float blue_component = original(i, j)[2];

            // Luminanzformel: Y = 0.299 * R + 0.587 * G + 0.114 * B
            const float luminance_value = 0.299f * red_component + 0.587f * green_component + 0.114f * blue_component;

            converted(i, j)[0] = luminance_value;
        }
    }

    return converted;
}

// Konvertiert ein Graustufenbild in ein reines Schwarz-Weiß-Bild
cg::image<cg::color_space_t::BW> cg::image_converter::gray_to_bw(const image<cg::color_space_t::Gray>& original)
{
    image<cg::color_space_t::BW> converted_image(original.get_width(), original.get_height());

    for (unsigned int row_index = 0; row_index < original.get_height(); ++row_index)
    {
        for (unsigned int col_index = 0; col_index < original.get_width(); ++col_index)
        {
            // Liest die Graustufen-Intensität des aktuellen Pixels aus dem Originalbild
            const float gray_intensity = original(col_index, row_index)[0];

            // Ein Wert unter 0.5 wird als Schwarz interpretiert, alle anderen Werte als Weiß
            if (gray_intensity < 0.5f)
            {
                converted_image(col_index, row_index)[0] = 0.0f; // Setzt den Pixel auf Schwarz
            }
            else
            {
                converted_image(col_index, row_index)[0] = 1.0f; // Setzt den Pixel auf Weiß
            }
        }
    }

    // Gibt das neu erstellte Schwarz-Weiß-Bild zurück
    return converted_image;
}