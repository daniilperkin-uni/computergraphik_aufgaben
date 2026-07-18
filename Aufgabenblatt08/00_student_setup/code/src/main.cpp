// OpenGL example application, that renders a bunch of trees via instancing.

#include "core.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lodepng/lodepng.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>

/// Combination of a vertex array object and an index buffer object.
struct RenderBatch {
    GLuint vao; // vertex array object
    GLuint ibo; // index buffer object
    int32_t index_count;
};

/// Layout of the vertices, used to store them in vertex buffer objects.
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

/// Layout of the per-frame data submitted to the graphics device.
struct PerFrame {
    glm::mat4 view; // world coords -> clip coords
    glm::mat4 proj;
};

/// Layout of the per-object data submitted to the graphics device.
struct PerObject {
    glm::mat4 model; // object coords -> world coords
};

RenderBatch CreateRenderBatch(const std::vector<Vertex> &vertices,
                              const std::vector<uint32_t> &indices);

GLuint CreateSpriteTexture(std::string const &filename);

void InitRenderer();
void InitScene();
void UpdateShaders();
int KeyAxisValue(GLFWwindow *window, int key1, int key2);
void applyJoystickInput();
bool NeedSceneUpdate();
void UpdateScene();
void RenderFrame();
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

GLFWwindow *g_window;
int g_window_width = 960, g_window_height = 540;

GLuint g_shader_program_ground = 0;        // shader program for rendering the ground
GLuint g_shader_program_trees = 0;         // shader program for rendering the trees
GLuint g_shader_program_tree_imposter = 0; // shader program for rendering the tree imposters

RenderBatch g_render_batch_ground;        // geometry of the ground
RenderBatch g_render_batch_tree;          // geometry of a tree
RenderBatch g_render_batch_tree_imposter; // geometry of a tree imposter

GLuint g_texture_name_tree_sprite; // texture for the tree imposters

GLuint g_ubo_per_frame; // uniform buffer containing per-frame data
GLuint g_ubo_trees;     // uniform buffer containing per-object data for all trees

PerFrame g_per_frame; // local copy of the per-frame uniform buffer

glm::vec3 g_cam_pos{-10, 5, 12};   // camera position
glm::vec3 g_cam_velocity{0, 0, 0}; // change of g_cam_pos per tick
float g_cam_yaw = -0.7f;           // camera left-right orientation in [-pi, pi]
float g_diff_cam_yaw = 0.f;        // change of g_cam_yaw per tick
float g_cam_pitch = -0.2f;         // camera up down orientation in [-1.5, 1.5]
float g_diff_cam_pitch = 0.f;      // change of g_cam_pitch per tick

constexpr int numTrees = 192; // number of trees to render
bool g_render_sprites = false;
/// Entry point into the OpenGL example application.
int main() {
    try {
        // initialize glfw
        GlfwInstance::init();

        // initialize this application
        InitRenderer();
        InitScene();

        // render until the window should close
        while (!glfwWindowShouldClose(g_window)) {
            glfwPollEvents(); // handle events
            while (NeedSceneUpdate())
                UpdateScene();         // per-tick updates
            RenderFrame();             // render image
            glfwSwapBuffers(g_window); // present image
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\nPress enter.\n";
        getchar();
        return -1;
    }
}

/// Creates the window and sets persistent render settings and compiles shaders.
/// @throw std::runtime_error If an error occurs during intialization.
void InitRenderer() {
    // create window
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    g_window = glfwCreateWindow(g_window_width, g_window_height, // initial resolution
                                "Geometrie",                     // window title
                                nullptr, nullptr);
    if (!g_window)
        throw std::runtime_error("Failed to create window.");

    // use the window that was just created
    glfwMakeContextCurrent(g_window);

    // get window resolution
    glfwGetFramebufferSize(g_window, &g_window_width, &g_window_height);

    // set callbacks for when the resolution changes or a key is pressed
    glfwSetFramebufferSizeCallback(g_window, &FramebufferSizeCallback);
    glfwSetKeyCallback(g_window, &KeyCallback);

    // enable vsync
    glfwSwapInterval(1);

    // load OpenGL (return value of 0 indicates error)
    if (!gladLoadGL(glfwGetProcAddress)) {
        throw std::runtime_error("Failed to load OpenGL context.");
    }
    std::cout << "OpenGL " << glGetString(GL_VERSION) << '\n';
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    // enable depth buffering
    glEnable(GL_DEPTH_TEST);

    // enable simple blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // create shader programs
    UpdateShaders();

    // create per-frame uniform buffer (will be filled later, once per frame)
    glCreateBuffers(1, &g_ubo_per_frame);
    glNamedBufferData(g_ubo_per_frame, sizeof(PerFrame), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_ubo_per_frame); // binding 0
}

/// Creates geometry and uniform buffers.
void InitScene() {
    // create ground geometry: each vertex consists of a position and a color
    std::vector<Vertex> ground_vertices{{{-20.5f, 0.0f, -20.5f}, {0.5f, 0.5f, 0.5f}},
                                        {{-20.5f, 0.0f, 20.5f}, {0.5f, 0.5f, 0.5f}},
                                        {{20.5f, 0.0f, 20.5f}, {0.5f, 0.5f, 0.5f}},
                                        {{20.5f, 0.0f, -20.5f}, {0.5f, 0.5f, 0.5f}}};
    std::vector<uint32_t> ground_indices{0, 1, 2, 0, 2, 3};
    g_render_batch_ground = CreateRenderBatch(ground_vertices, ground_indices);

    // TODO 1.3
    // Baum-Geometrie erstellen
    std::vector<Vertex> tree_vertices{
        {{-0.2f, 0.0f, 0.0f}, {0.4f, 0.2f, 0.1f}}, // Stamm
        {{ 0.2f, 0.0f, 0.0f}, {0.4f, 0.2f, 0.1f}},
        {{ 0.2f, 0.8f, 0.0f}, {0.4f, 0.2f, 0.1f}},
        {{-0.2f, 0.8f, 0.0f}, {0.4f, 0.2f, 0.1f}},
        {{-1.0f, 0.8f, 0.0f}, {0.0f, 0.5f, 0.0f}}, // Blätter
        {{ 1.0f, 0.8f, 0.0f}, {0.0f, 0.5f, 0.0f}},
        {{ 0.0f, 3.5f, 0.0f}, {0.0f, 0.5f, 0.0f}}
    };
    std::vector<uint32_t> tree_indices{0, 1, 2, 0, 2, 3, 4, 5, 6};
    g_render_batch_tree = CreateRenderBatch(tree_vertices, tree_indices);

    //TODO 2.1
    // Baum-Sprite-Geometrie erstellen: ein Quad im Bereich [-1.5 , 1.5] x [0.0 3.0]
    // und mit Texturkoordinaten im Bereich [0,1] x [1,0]
    // Das Farbattribut der Vertices verwenden, um die Texturkoordinaten zu speichern (einfach den
    // dritten Eintrag auf 0 setzen)
    std::vector<Vertex> imposter_vertices{
        {{-1.5f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 1.5f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
        {{ 1.5f, 3.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{-1.5f, 3.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    };
    std::vector<uint32_t> imposter_indices{0, 1, 2, 0, 2, 3};
    g_render_batch_tree_imposter = CreateRenderBatch(imposter_vertices, imposter_indices);

    // create tree sprite texture
    try {
        g_texture_name_tree_sprite = CreateSpriteTexture("../resources/pine_tree_sprite.png");
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;        
    }

    // create transformations used for instanced rendering of the trees
    std::array<glm::mat4, numTrees> model_trees;
    std::default_random_engine generator;
    std::uniform_int_distribution<> dist(-20, 20);
    for (auto &model : model_trees) {
        int x = dist(generator);
        int z = dist(generator);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z));
    }

    // create & fill uniform buffer holding the transformations
    glCreateBuffers(1, &g_ubo_trees);
    glNamedBufferData(g_ubo_trees, sizeof(PerObject) * model_trees.size(), model_trees.data(),
        GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, g_ubo_trees);
}

/// @param vertices A vertex buffer.
/// @param indices An index buffer.
/// @return RenderBatch object created using the given buffers.
RenderBatch CreateRenderBatch(const std::vector<Vertex> &vertices,
                              const std::vector<uint32_t> &indices) {
    RenderBatch job;
    job.index_count = static_cast<int32_t>(indices.size());


    // create, bind, and fill index buffer using job.ibo to store the GL handle
    // NOTE: we are not using direct state access (DSA) functions here. 
    // The buffer is initialized and filled via binding to the GL_ELEMENT_ARRAY_BUFFER buffer target.
    // GL_ELEMENT_ARRAY_BUFFER target is used to store vertex array indices.
    // See https://registry.khronos.org/OpenGL-Refpages/gl4 for details on the individual gl*(...) functions.
    glGenBuffers(1, &job.ibo);  // create the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, job.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(uint32_t) * indices.size(), // data size
                 indices.data(),                    // data pointer
                 GL_STATIC_DRAW);                   // usage
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // TODO 1.2
    // Nur DSA-Funktionen verwenden, um:
    // a) den Vertex-Buffer zu erstellen und zu füllen
    // b) das Vertex-Array-Objekt zu erstellen und zu initialisieren
    // c) das Index-Buffer-Objekt (IBO) und das Vertex-Buffer-Objekt (VBO) an das Vertex-Array-Objekt (VAO) zu binden
    // d) die Vertex-Array-Attribute zu aktivieren und zu spezifizieren. 
    
    // a) Ein Buffer-Objekt erstellen und füllen (das wir später als Vertex-Buffer verwenden).
    // Die lokale Variable vbo kann für das GL-Handle verwendet werden, da sie bei der späteren Verwendung mit dem Vertex-Array-Objekt verknüpft wird. 
    GLuint vbo;  // Variable zum Speichern des (Vertex-)Buffer-Objekt-Handles
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // b) Das Vertex-Array-Objekt erstellen und initialisieren und das Handle in job.vao speichern
    glCreateVertexArrays(1, &job.vao);
    
    // c) Die IBO- und VBO-Buffer an das VAO binden und deren Verwendung durch Wahl der korrekten API-Funktionen spezifizieren (Element-Buffer vs. Vertex-Buffer)
    glVertexArrayElementBuffer(job.vao, job.ibo);
    glVertexArrayVertexBuffer(job.vao, 0, vbo, 0, sizeof(Vertex));
    
    // d) Die Vertex-Attribute für Vertex-Position und -Farbe auf dem VAO aktivieren
    glEnableVertexArrayAttrib(job.vao, 0);
    glEnableVertexArrayAttrib(job.vao, 1);
     
    // Das Layout jedes Attributs spezifizieren (Attribut-Index 0 für Position und Index 1 für Farbe verwenden)
    glVertexArrayAttribFormat(job.vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(job.vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));

    // Den Vertex-Buffer des VAO spezifizieren, auf den sich die Positions- und Farbattribute beziehen
    glVertexArrayAttribBinding(job.vao, 0, 0);
    glVertexArrayAttribBinding(job.vao, 1, 0);

    // END TODO 1.2

    return job;
}

/// Creates an OpenGL texture object using a texture from a file.
/// @param filename Path to the file containing the texture.
/// @return OpenGL handle of the created texture object.
GLuint CreateSpriteTexture(std::string const &filename) {
    // Load file and decode image.
    unsigned int width, height;
    std::vector<unsigned char> image_data;
    unsigned int error = lodepng::decode(image_data, width, height, filename);

    if (error != 0) {
        throw std::invalid_argument("Could not load resource file.");
    }

    GLint level = 0;
    GLint internal_format = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    GLint border = 0;

    GLuint texture_name = 0;

    // TODO 2.3

    // Ein neues GL_TEXTURE_2D Texturobjekt mittels glCreateTextures erstellen und initialisieren und seinen Namen in texture_name speichern
    glCreateTextures(GL_TEXTURE_2D, 1, &texture_name);

    // glTextureParameter verwenden, um den Texture-Wrap-Modus auf GL_MIRRORED_REPEAT,
    // den Minifizierungsfilter auf GL_LINEAR_MIPMAP_LINEAR und den Magnifizierungsfilter auf GL_LINEAR zu setzen
    // Hinweis: Es sind vier Aufrufe der Funktion erforderlich
    glTextureParameteri(texture_name, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTextureParameteri(texture_name, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTextureParameteri(texture_name, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture_name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Die Funktion glTextureStorage2D verwenden, um den Bildspeicher (mit bisher uninitialisierten Pixeldaten) zu spezifizieren, der zum Speichern der tatsächlichen Texturdaten verwendet wird
    GLsizei levels = static_cast<GLsizei>(std::floor(std::log2(std::max(width, height)))) + 1;
    glTextureStorage2D(texture_name, levels, internal_format, width, height);

    // Die Funktion glTextureSubImage2D verwenden, um den Bildspeicher mit den oben geladenen Bilddaten zu füllen
    glTextureSubImage2D(texture_name, 0, 0, 0, width, height, format, type, image_data.data());

    // Mipmaps für die Textur mittels glGenerateTextureMipmap generieren
    glGenerateTextureMipmap(texture_name);

    // END TODO 2.3

    return texture_name;
}

/// Loads the shader programs or reloads them.
void UpdateShaders() {
    glDeleteProgram(g_shader_program_ground);
    glDeleteProgram(g_shader_program_trees);

    try {
        // compile vertex & fragment shader
        GLuint vertex_shader_ground = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vertex_shader_ground, "../shaders/vert_ground.glsl");
        GLuint vertex_shader_trees = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vertex_shader_trees, "../shaders/vert_trees.glsl");
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(fragment_shader, "../shaders/frag.glsl");

        GLuint vertex_shdr_tree_sprite = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vertex_shdr_tree_sprite, "../shaders/vert_tree_imposter.glsl");
        GLuint fragment_shdr_tree_sprite = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(fragment_shdr_tree_sprite, "../shaders/frag_tree_imposter.glsl");

        // link shaders
        g_shader_program_ground = glCreateProgram();
        LinkProgram(g_shader_program_ground, {vertex_shader_ground, fragment_shader});
        g_shader_program_trees = glCreateProgram();
        LinkProgram(g_shader_program_trees, {vertex_shader_trees, fragment_shader});
        g_shader_program_tree_imposter = glCreateProgram();
        LinkProgram(g_shader_program_tree_imposter,
                    {vertex_shdr_tree_sprite, fragment_shdr_tree_sprite});

        // clean shaders up
        glDetachShader(g_shader_program_trees, fragment_shader);
        glDetachShader(g_shader_program_trees, vertex_shader_trees);
        glDetachShader(g_shader_program_ground, fragment_shader);
        glDetachShader(g_shader_program_ground, vertex_shader_ground);
        glDetachShader(g_shader_program_tree_imposter, vertex_shdr_tree_sprite);
        glDetachShader(g_shader_program_tree_imposter, fragment_shdr_tree_sprite);
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader_trees);
        glDeleteShader(vertex_shader_ground);
        glDeleteShader(vertex_shdr_tree_sprite);
        glDeleteShader(fragment_shdr_tree_sprite);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << "\nPress 'R' to reload the shaders.\n";
    }
}

/// Helper function for controlling an axis with 2 keys.
/// @param window The window where the keys should be pressed.
/// @param key1 The first key.
/// @param key2 The second key.
/// @return -1 if only the first key is pressed, 1 if only the second key is pressed and 0 if none
/// of the keys or both keys are pressed.
int KeyAxisValue(GLFWwindow *window, int key1, int key2) {
    bool key1_pressed = glfwGetKey(g_window, key1) == GLFW_PRESS;
    bool key2_pressed = glfwGetKey(g_window, key2) == GLFW_PRESS;
    return key2_pressed - key1_pressed;
}

double t0, t1 = 0.0;

void applyJoystickInput()
{
	if (!glfwJoystickPresent(GLFW_JOYSTICK_1))
		return;

	t0 = t1;
	t1 = glfwGetTime();
	double dt = t1 - t0;

	int count;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

	// update yaw and pitch
	float rot_dx = axes[2];
	float rot_dy = axes[3];

	if (abs(rot_dx) < 0.025f)
		rot_dx = 0.0f;

	if (abs(rot_dy) < 0.025f)
		rot_dy = 0.0f;

	rot_dx = std::abs(rot_dx) * rot_dx * 1.5f;
	rot_dy = std::abs(rot_dy) * rot_dy * 1.5f;

	g_cam_yaw += -rot_dx * static_cast<float>(dt);
	g_cam_yaw = std::remainder(g_cam_yaw, 2 * glm::pi<float>());
	g_cam_pitch += rot_dy * static_cast<float>(dt);
	g_cam_pitch = glm::clamp(g_cam_pitch, -1.5f, 1.5f);


	float trans_dx = axes[0];
	float trans_dy = axes[1];

	if (abs(trans_dx) < 0.05f)
		trans_dx = 0.0f;

	if (abs(trans_dy) < 0.05f)
		trans_dy = 0.0f;

	glm::vec3 position = g_cam_pos;

	// calculate camera direction
	glm::vec3 vfront{ 0, 0, -1 };
	vfront = glm::mat3(glm::rotate(glm::mat4(1.0f), g_cam_pitch, glm::vec3(1, 0, 0))) * vfront;
	vfront = glm::mat3(glm::rotate(glm::mat4(1.0f), g_cam_yaw, glm::vec3(0, 1, 0))) * vfront;

	glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 vRighthand = glm::normalize(glm::cross(vfront, vUp));

	g_cam_pos = position + trans_dy * 20.0f * vfront * (float)dt + trans_dx * 20.0f * vRighthand * (float)dt;
}

/// Synchronizes scene updates to a frequenzy of 100 Hz.
/// This function assumes that the scene is updated each time it returns true.
/// @return true iff the scene should be updated now.
bool NeedSceneUpdate() {
    static auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    if (nextUpdate > now)
        return false;
    nextUpdate += std::chrono::milliseconds{10};
    return true;
}

/// Applies per-frame updates of the scene.
void UpdateScene() {

	// update camera postion and orientation from gamepad input
	applyJoystickInput();

    // update camera yaw and pitch
    int rot_horz_target = KeyAxisValue(g_window, GLFW_KEY_RIGHT, GLFW_KEY_LEFT);
    int rot_vert_target = KeyAxisValue(g_window, GLFW_KEY_DOWN, GLFW_KEY_UP);
    g_diff_cam_yaw = lerp(0.1f, g_diff_cam_yaw, static_cast<float>(rot_horz_target));
    g_diff_cam_pitch = lerp(0.1f, g_diff_cam_pitch, static_cast<float>(rot_vert_target));
    g_cam_yaw += 0.03f * g_diff_cam_yaw;
    g_cam_yaw = std::remainder(g_cam_yaw, 2 * glm::pi<float>());
    g_cam_pitch += 0.03f * g_diff_cam_pitch;
    g_cam_pitch = glm::clamp(g_cam_pitch, -1.5f, 1.5f);

    // calculate camera direction
    glm::vec3 cam_dir{0, 0, -1};
    cam_dir = glm::mat3(glm::rotate(glm::mat4(1.0f), g_cam_pitch, glm::vec3(1, 0, 0))) * cam_dir;
    cam_dir = glm::mat3(glm::rotate(glm::mat4(1.0f), g_cam_yaw, glm::vec3(0, 1, 0))) * cam_dir;

    // calculate the rotation matrix used to orient the target velocity along the camera
    glm::vec2 cam_dir_2d{cam_dir.x, cam_dir.z}; // only consider the xz-plane
    cam_dir_2d = glm::normalize(cam_dir_2d);
    glm::mat3 rot{-cam_dir_2d.y, 0, cam_dir_2d.x, 0, 1, 0, -cam_dir_2d.x, 0, -cam_dir_2d.y};

    // update camera velocity and position
    glm::vec3 target_velocity =
        rot * glm::vec3{KeyAxisValue(g_window, GLFW_KEY_A, GLFW_KEY_D),
                        KeyAxisValue(g_window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE),
                        KeyAxisValue(g_window, GLFW_KEY_W, GLFW_KEY_S)};
    g_cam_velocity = lerp(0.08f, g_cam_velocity, target_velocity);
    g_cam_pos += 0.05f * g_cam_velocity;

    // TODO 1.1
    // View-Matrix berechnen und g_per_frame.view zuweisen
    g_per_frame.view = glm::lookAt(g_cam_pos, g_cam_pos + cam_dir, glm::vec3(0, 1, 0));

    // Projektionsmatrix berechnen und g_per_frame.proj zuweisen
    float aspect = static_cast<float>(g_window_width) / static_cast<float>(g_window_height);
    g_per_frame.proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);
}

/// Renders a frame.
void RenderFrame() {
    // update per-frame uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, g_ubo_per_frame);
    void *data = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(data, &g_per_frame, sizeof(g_per_frame));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // setup rendering
    glViewport(0, 0, g_window_width, g_window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render the ground
    glUseProgram(g_shader_program_ground);
    glBindVertexArray(g_render_batch_ground.vao);
    glDrawElements(GL_TRIANGLES, g_render_batch_ground.index_count, GL_UNSIGNED_INT, nullptr);

    // TODO 2.2
    if (!g_render_sprites) { // change this globally at top of file
        // render the trees
        glUseProgram(g_shader_program_trees);
        glBindVertexArray(g_render_batch_tree.vao);
        glDrawElementsInstanced(GL_TRIANGLES, g_render_batch_tree.index_count, GL_UNSIGNED_INT,
            nullptr, numTrees);
    }
    else {
        // render the tree sprites
        glUseProgram(g_shader_program_tree_imposter);
        glBindTextureUnit(0, g_texture_name_tree_sprite);  // bind to texture unit 0
        glUniform1i(glGetUniformLocation(g_shader_program_tree_imposter, "tree_sprite_tx2D"), 0); // set shader uniform to match texture unit (index 0)
        glBindVertexArray(g_render_batch_tree_imposter.vao);
        glDrawElementsInstanced(GL_TRIANGLES, g_render_batch_tree_imposter.index_count, GL_UNSIGNED_INT,
            nullptr, numTrees);
    }

    // unbind buffers
    glBindVertexArray(0);
}

/// Updates the resolution. This is called by GLFW when the resolution changes.
/// @param window The window that changed resolution.
/// @param width New width.
/// @param height New height.
void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    g_window_width = width;
    g_window_height = height;
}

/// Reloads the shaders if 'R' is pressed. This is called by GLFW when a key is pressed.
/// @param window The window in which a key was pressed.
/// @param key GLFW key code of the pressed key.
/// @param scancode platform-specific key code.
/// @param action One of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
/// @param mods Modifier bits.
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        UpdateShaders();
}
