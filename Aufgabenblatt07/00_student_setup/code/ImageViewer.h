#ifndef ImageViewer_h
#define ImageViewer_h

#include <memory>

#include "Image.h"
#include "ImageFilter.h"
#include "ImageIO.h"

#include "glowl/Texture2D.hpp"
#include "glowl/GLSLProgram.hpp"

struct GLFWwindow;

namespace cg
{
    /** Simple OpenGL ImageViewer class */
    class ImageViewer
    {
    public:
        enum ComputeMode { CPU, GPU };
        enum FilterMode { CPU_GAUSSIAN_2D, CPU_SEPERATED_GAUSSIAN, GPU_SEPERATED_GAUSSIAN, CPU_EDGE_DETECTION };

        ImageViewer();

        /** Starts the OpenGL ImageViewer. Returns only after the window is closed. */
        void run();

    private:
        /** Pointer to active window */
        GLFWwindow* m_active_window;

        /** CPU representation of the loaded image */
        image<color_space_t::RGBA> m_original_image;

        /** Path to loaded image */
        std::string m_image_path;

        /** GLSL shader program for displaying the image */
        std::unique_ptr<GLSLProgram> m_display_prgm;
        /** GLSL shader program for filtering the image on the GPU */
        std::unique_ptr<GLSLProgram> m_filter_prgm;
        /** OpenGL texture objects for storing and working with the image data on the GPU */
        std::array<std::unique_ptr<Texture2D>, 3> m_textures;

        /**************************************************************************
        * Filter configuration state
        *************************************************************************/

        /** Active compute device */
        ComputeMode m_compute_mode;
        /** Active filter mode */
        FilterMode m_active_mode;
        /** Active filter border policy */
        filter::BorderPolicy m_active_border_policy;
        /** Extents of filter (if not fixed size filter) */
        std::pair<int, int> m_extents;
        /** Sigma value for gaussian filters */
        float m_sigma;

        /**************************************************************************
        * Private helper functions
        *************************************************************************/

        void drawUI();

        void updateDisplayImage();

        void applyCPUGaussian2D();

        void applyCPUSeperatedGaussian();

        void applyGPUSeperatedGaussian();

        void applyCPUEdgeDetection();

        /**************************************************************************
        * (Static) callbacks functions
        *************************************************************************/

        static void windowSizeCallback(GLFWwindow* window, int width, int height);
        static void windowCloseCallback(GLFWwindow* window);
    };

}

#endif // !ImageViewer_h
