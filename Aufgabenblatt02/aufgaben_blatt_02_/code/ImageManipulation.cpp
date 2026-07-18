#include "ImageManipulation.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

//Modifiziert ein Bild im HSV-Farbraum nach einem spezifischen Satz von Regeln
cg::image<cg::color_space_t::HSV> cg::image_manipulation::modify_in_hsv(const image<color_space_t::HSV>& original)
{
    // Erstelle eine Kopie des Originalbildes mit den gleichen Abmessungen
    // Dieses Bild wird die modifizierten Pixel aufnehmen
    cg::image<cg::color_space_t::HSV> modified_image(original.get_width(), original.get_height());

    // Iteriere über jeden Pixel des Bildes, Zeile für Zeile (j) und Spalte für Spalte (i)
    for (unsigned int row_index = 0; row_index < original.get_height(); ++row_index)
    {
        for (unsigned int col_index = 0; col_index < original.get_width(); ++col_index)
        {

            // Extrahiere die HSV-Werte des aktuellen Pixels aus dem Originalbild
            const float original_hue = original(col_index, row_index)[0];
            const float original_saturation = original(col_index, row_index)[1];
            const float original_value = original(col_index, row_index)[2];

            // Initialisiere die modifizierten Werte mit den Originalwerten
            float modified_hue = original_hue;
            float modified_saturation = original_saturation;
            float modified_value = original_value;

            // Farbtonrotation
            float hue_in_degrees = original_hue * 360.0f;

            // Addiere 30 Grad zum Farbton, um eine Rotation im Farbkreis zu erzeugen
            hue_in_degrees += 30.0f;

            // Stelle sicher, dass der Wert im Bereich [0°, 360°] bleibt
            if (hue_in_degrees >= 360.0f) {
                hue_in_degrees -= 360.0f;
            }

            // Konvertiere den rotierten Farbton zurück in den normalisierten Bereich [0, 1]
            modified_hue = hue_in_degrees / 360.0f;


            // Bedingte Anpassung von Sättigung und Helligkeit
            if (hue_in_degrees >= 50.0f && hue_in_degrees <= 100.0f)
            {
                modified_saturation = original_saturation * 0.9f;

                modified_value = original_value * 0.7f;
            }
            else
            {

                modified_saturation = 0.0f;

                modified_value = original_value * 0.8f;
            }

            // Modifizierte Werte zurückschreiben
            modified_image(col_index, row_index)[0] = modified_hue;
            modified_image(col_index, row_index)[1] = modified_saturation;
            modified_image(col_index, row_index)[2] = modified_value;
        }
    }
    return modified_image;
}