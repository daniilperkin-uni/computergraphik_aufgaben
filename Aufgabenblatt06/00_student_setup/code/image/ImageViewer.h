#pragma once

#include <memory>

#include "../Rasterizer.h"

#include "Image.h"

#include <Texture2D.hpp>
#include <GLSLProgram.hpp>

struct GLFWwindow;

namespace cg
{
    /** Simple OpenGL ImageViewer class */
    class ImageViewer
    {
    public:
        /** Constructor */
        ImageViewer(Rasterizer& rasterizer);

        /** Starts the OpenGL ImageViewer. Returns only after the window is closed. */
        void run();

    private:
        /** Pointer to active window */
        GLFWwindow* m_active_window;

        /** CPU representation of the loaded image */
        const image<color_space_t::RGBA>& m_image;

        /** GLSL shader program for displaying the image */
        std::unique_ptr<GLSLProgram> m_display_prgm;

        /** OpenGL texture objects for storing and working with the image data on the GPU */
        std::unique_ptr<Texture2D> m_texture;

        /** GUI options */
        bool m_rotate;
        bool m_needs_update;
        bool m_auto_update;

        /** Draw UI */
        void drawUI();

        /** Rasterizer for calling the draw method and additional GUI options */
        Rasterizer& rasterizer;

        /** Callbacks for window events */
        void windowSizeCallback(GLFWwindow* window, int width, int height);
        void windowCloseCallback(GLFWwindow* window);
    };
}
