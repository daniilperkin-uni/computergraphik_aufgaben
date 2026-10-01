#include "ImageViewer.h"

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "../scene/Objects/Container.h"
#include "glm/vec3.hpp"

namespace cg
{
    namespace
    {
        /** Check for existing file */
        bool check_file(const std::string& name) {
            std::ifstream f(name.c_str());
            return f.good();
        }

        /**
        * \brief Read a shader source file
        * \param path Location of the shader file
        * \return Returns a string containing the shader source
        */
        std::string readShaderFile(const char* const path)
        {
            std::ifstream inFile(path, std::ios::in);

            std::ostringstream source;
            while (inFile.good()) {
                int c = inFile.get();
                if (!inFile.eof()) source << (char)c;
            }
            inFile.close();

            return source.str();
        }
    }

    ImageViewer::ImageViewer(Rasterizer& rasterizer)
        : m_image(rasterizer.accessImage()), rasterizer(rasterizer), m_needs_update(true), m_auto_update(false)
    { }

    void ImageViewer::run()
    {
        std::cout << "OpenGL ImageViewer" << std::endl;

        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Error: Couldn't initialize glfw." << std::endl;
            return;
        }

#if defined(__APPLE__) || defined(DOWNGRADE_GL)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

        this->m_active_window = glfwCreateWindow(this->m_image.get_width(), this->m_image.get_height(), "Rasterizer", NULL, NULL);

        if (!this->m_active_window)
        {
            std::cerr << "Error: Couldn't open glfw window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(this->m_active_window);

        // Register callback functions
        auto windowSizeCallbackStub = [](GLFWwindow* w, int width, int height)
        {
            static_cast<ImageViewer*>(glfwGetWindowUserPointer(w))->windowSizeCallback(w, width, height);
        };

        auto windowCloseCallbackStub = [](GLFWwindow* w)
        {
            static_cast<ImageViewer*>(glfwGetWindowUserPointer(w))->windowCloseCallback(w);
        };

        glfwSetWindowSizeCallback(this->m_active_window, windowSizeCallbackStub);
        glfwSetWindowCloseCallback(this->m_active_window, windowCloseCallbackStub);

        glfwSetWindowUserPointer(this->m_active_window, this);

        // initialize imgui and install callbacks
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_active_window, true);
        ImGui_ImplOpenGL3_Init();

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cerr << "Something went wrong!" << std::endl;
            return;
        }

        std::cout << "GPU details:" << std::endl;
        std::cout << "    Vendor:         " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "    GPU:            " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "    OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "    GLSL version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        // Load display texture shader program
        std::string display_shader_frag_src;
        std::string display_shader_vert_src;

#if defined(__APPLE__) || defined(DOWNGRADE_GL)
        if (check_file("../../shader/display_texture_v_downgrade.glsl") && check_file("../../shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("../../shader/display_texture_v_downgrade.glsl");
            display_shader_frag_src = readShaderFile("../../shader/display_texture_f.glsl");
        }
        else if (check_file("../shader/display_texture_v_downgrade.glsl") && check_file("../shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("../shader/display_texture_v_downgrade.glsl");
            display_shader_frag_src = readShaderFile("../shader/display_texture_f.glsl");
        }
        else if (check_file("shader/display_texture_v_downgrade.glsl") && check_file("shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("shader/display_texture_v_downgrade.glsl");
            display_shader_frag_src = readShaderFile("shader/display_texture_f.glsl");
        }
        else
        {
            std::cerr << "Could not located display_texture_*.glsl shader source files." << std::endl;
            return;
        }
#else
        if (check_file("../../shader/display_texture_v.glsl") && check_file("../../shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("../../shader/display_texture_v.glsl");
            display_shader_frag_src = readShaderFile("../../shader/display_texture_f.glsl");
        }
        else if (check_file("../shader/display_texture_v.glsl") && check_file("../shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("../shader/display_texture_v.glsl");
            display_shader_frag_src = readShaderFile("../shader/display_texture_f.glsl");
        }
        else if (check_file("shader/display_texture_v.glsl") && check_file("shader/display_texture_f.glsl"))
        {
            display_shader_vert_src = readShaderFile("shader/display_texture_v.glsl");
            display_shader_frag_src = readShaderFile("shader/display_texture_f.glsl");
        }
        else
        {
            std::cerr << "Could not locate display_texture_*.glsl shader source files." << std::endl;
            return;
        }
#endif

        this->m_display_prgm = std::make_unique<GLSLProgram>();
        bool prgm_error = false;
        prgm_error |= !this->m_display_prgm->compileShaderFromString(&display_shader_vert_src, GL_VERTEX_SHADER);
        prgm_error |= !this->m_display_prgm->compileShaderFromString(&display_shader_frag_src, GL_FRAGMENT_SHADER);
        prgm_error |= !this->m_display_prgm->link();
        if (prgm_error)
        {
            std::cout << "Error during shader program creation of 'display_texture_*.glsl':" << std::endl;
            std::cout << this->m_display_prgm->getLog();
        }

        while (!glfwWindowShouldClose(this->m_active_window))
        {
            TextureLayout img_layout(GL_RGBA32F, this->m_image.get_width(), this->m_image.get_height(), 1, GL_RGBA, GL_FLOAT, 1);
#if defined(__APPLE__) || defined(DOWNGRADE_GL)
            img_layout.int_parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
#else
            img_layout.int_parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR });
#endif
            img_layout.int_parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
            this->m_texture = std::make_unique<Texture2D>(img_layout, const_cast<std::array<float, 4>*>(this->m_image.data()));

#if defined(__APPLE__) || defined(DOWNGRADE_GL)
            const float vertices[6][4] = { { -1.0,-1.0,0.0,0.0 },
                                  { 1.0,1.0,1.0,1.0 },
                                  { -1.0,1.0,0.0,1.0 },
                                  { 1.0,1.0,1.0,1.0 },
                                  { -1.0,-1.0,0.0,0.0 },
                                  { 1.0,-1.0,1.0,0.0 } };

            GLuint vertex_buf = 0;
            glGenBuffers(1, &vertex_buf);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buf);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
            GLuint vao = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
#endif

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            int width, height;
            glfwGetFramebufferSize(this->m_active_window, &width, &height);
            glViewport(0, 0, width, height);

            this->m_display_prgm->use();

            glActiveTexture(GL_TEXTURE0);
            this->m_texture->bindTexture();
            glUniform1i(this->m_display_prgm->getUniformLocation("display_tx2D"), 0);

#if defined(__APPLE__) || defined(DOWNGRADE_GL)
            glBindVertexArray(vao);
#endif

            glDrawArrays(GL_TRIANGLES, 0, 6);

#if defined(__APPLE__) || defined(DOWNGRADE_GL)
            glBindVertexArray(0);
#endif

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            drawUI();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            auto gl_err = glGetError();
            if (gl_err != GL_NO_ERROR)
                std::cerr << "GL error at end of frame: " << gl_err << std::endl;

            glfwSwapBuffers(this->m_active_window);
            glfwPollEvents();
        }

        // Clean up GPU resources while context is still alive
        this->m_display_prgm.reset();
    }

    void ImageViewer::drawUI()
    {
        ImGuiWindowFlags window_flags = 0;
        bool p_open = true;
        if (!ImGui::Begin("Image Viewer", &p_open, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        // Scene selection
        ImGui::Text("Scene");

        for (int i = 0; i < this->rasterizer.getNumScenes() - 1; ++i)
        {
            if (ImGui::RadioButton(this->rasterizer.getScenes()[i].getName().c_str(), this->rasterizer.accessActiveScene() == i))
            {
                if (this->rasterizer.accessActiveScene() != i)
                {
                    this->m_needs_update = true;
                }

                this->rasterizer.accessActiveScene() = i;
            }

            ImGui::SameLine();
        }

        if (ImGui::RadioButton(this->rasterizer.getScenes()[this->rasterizer.getNumScenes() - 1].getName().c_str(),
            this->rasterizer.accessActiveScene() == this->rasterizer.getNumScenes() - 1))
        {
            if (this->rasterizer.accessActiveScene() != this->rasterizer.getNumScenes() - 1)
            {
                this->m_needs_update = true;
            }

            this->rasterizer.accessActiveScene() = this->rasterizer.getNumScenes() - 1;
        }

        ImGui::Separator();

        // Camera settings
        ImGui::Text("Camera");

        static float fov = this->rasterizer.accessCamera().getFov();
        static float zNear = this->rasterizer.accessCamera().getNear();
        static float zFar = this->rasterizer.accessCamera().getFar();

        ImGui::DragFloat("Field of view", &fov, 1.0f, 10.0f, 170.0f);
        ImGui::DragFloat("Near plane", &zNear, 0.01f, 0.001f, 10.0f);
        ImGui::DragFloat("Far plane", &zFar, 10.0f, 10.0f, 100000.0f);


        if (ImGui::Button("Reset", ImVec2(300, 20)))
        {
            this->rasterizer.accessCamera() = defaultCamera();
            this->rasterizer.accessCamera().setAspect(static_cast<float>(this->m_image.get_width()) / static_cast<float>(this->m_image.get_height()));

            fov = this->rasterizer.accessCamera().getFov();
            zNear = this->rasterizer.accessCamera().getNear();
            zFar = this->rasterizer.accessCamera().getFar();

            this->m_needs_update = true;
        }

        ImGui::Separator();

        // Rasterizer settings
        ImGui::Text("Rasterization");

        const char* items[] = { "POINTS", "WIREFRAME", "FILLED" };
        static int item = this->rasterizer.accessMode();
        ImGui::Combo("Mode", &item, items, IM_ARRAYSIZE(items));
        this->rasterizer.accessMode() = static_cast<Rasterizer::rasterization_mode>(item);

        ImGui::Separator();

        // Rotate option
        ImGui::Text("Rotation animation");

        ImGui::SameLine();
        if (ImGui::RadioButton("On", this->m_rotate))
        {
            this->m_rotate = true;
        }

        ImGui::SameLine();
        if (ImGui::RadioButton("Off", !this->m_rotate))
        {
            this->m_rotate = false;
        }

        float axis[3] = { this->rasterizer.accessRotationAxis()[0], this->rasterizer.accessRotationAxis()[1], this->rasterizer.accessRotationAxis()[2] };
        ImGui::DragFloat3("Rotation axis", axis, 0.01f, 0.0f, 1.0f);
        this->rasterizer.accessRotationAxis() = glm::vec3(axis[0], axis[1], axis[2]);

        ImGui::DragFloat("Rotation speed", &this->rasterizer.accessRotationSpeed(), 1.0f, 1.0f, 100.0f);

        ImGui::Separator();

        // Light options
        auto lights = this->rasterizer.accessScene().getLights();

        if (lights.size() != 0)
        {
            ImGui::Text("Lights");

            for (std::size_t i = 0; i < lights.size(); ++i)
            {
                const std::string object_name = "Use " + lights[i]->getShapeName() + " #" + std::to_string(i);
                const std::string color_name = "Light color " + std::to_string(i);
                const std::string intensity_name = "Light intensity " + std::to_string(i);

                ImGui::Checkbox(object_name.c_str(), &lights[i]->accessVisibility());

                float color[3] = { lights[i]->accessColor()[0], lights[i]->accessColor()[1], lights[i]->accessColor()[2] };
                ImGui::ColorEdit3(color_name.c_str(), color);
                lights[i]->accessColor() = Color(color[0], color[1], color[2], 1.0f);

                ImGui::DragFloat(intensity_name.c_str(), &lights[i]->accessIntensity(), 0.1f, 0.0f, 100.0f);
            }

            ImGui::Separator();
        }

        // Object options
        auto objects = this->rasterizer.accessScene().getObjects();

        if (objects.size() != 0)
        {
            ImGui::Text("Objects");

            for (std::size_t i = 0; i < objects.size(); ++i)
            {
                const std::string object_name = "Show " + objects[i]->getShapeName() + " #" + std::to_string(i);
                const std::string color_name = "Object color " + std::to_string(i);

                ImGui::Checkbox(object_name.c_str(), &objects[i]->accessVisibility());

                if (dynamic_cast<cg::Container*>(objects[i].get()) == nullptr)
                {
                    float color[3] = { objects[i]->accessColor()[0], objects[i]->accessColor()[1], objects[i]->accessColor()[2] };
                    ImGui::ColorEdit3(color_name.c_str(), color);
                    objects[i]->accessColor() = Color(color[0], color[1], color[2], 1.0f);
                }
            }

            ImGui::Separator();
        }

        // Draw button
        ImGui::Checkbox("Auto update", &this->m_auto_update);

        if (ImGui::Button("Update", ImVec2(300, 20)) || this->m_rotate || this->m_needs_update || this->m_auto_update)
        {
            this->rasterizer.accessCamera().setFov(fov);
            this->rasterizer.accessCamera().setNear(zNear);
            this->rasterizer.accessCamera().setFar(zFar);

            this->rasterizer.draw(this->m_rotate);
            this->m_needs_update = this->m_rotate;
        }

        ImGui::End();
    }

    void ImageViewer::windowSizeCallback(GLFWwindow* window, int width, int height)
    {
        this->rasterizer.accessCamera().setAspect(static_cast<float>(width) / static_cast<float>(height));
        this->rasterizer.accessImage().resize(width, height);

        this->m_needs_update = true;
    }

    void ImageViewer::windowCloseCallback(GLFWwindow* window)
    { }
}
