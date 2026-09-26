// OpenGL example application, that renders a small scene

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <memory>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <tiny_gltf.h>

#include "core.hpp"
#include "scene.hpp"


struct VertexBufferDescriptor {
    unsigned char* data;
    size_t byte_size;

    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei offset;
    GLsizei stride;

    std::string semantic_name;
};

struct IndexBufferDescriptor {
    unsigned char* data;
    size_t byte_size;

    GLenum type;
};

struct TextureDescriptor {
    unsigned char* data;

    GLint internal_format;
    int width;
    int height;
    int depth;
    GLenum format;
    GLenum type;

    GLsizei levels;

    std::vector<std::pair<GLenum, GLint>> int_parameters;

    std::string semantic_name;
};

/// Layout of the per-frame data submitted to the graphics device.
struct PerFrame {
    glm::mat4 view; // world coords -> clip coords
    glm::mat4 proj;
    std::array<glm::vec4, 4> light_positions;
    std::array<glm::vec4, 4> light_color_intensity;
};

MeshComponent CreateMesh(std::vector<VertexBufferDescriptor> const& vertices, IndexBufferDescriptor const& indices,
    std::vector<TextureDescriptor> const& textures);

void addGltfNode(
    Scene& scene, std::shared_ptr<tinygltf::Model> const& gltf_model, int gltf_node_idx, std::size_t parent_transform_idx);
void importGltfScene(Scene& scene, std::string const& gltf_filepath);

void InitRenderer();
void InitScene(std::string const& scene_path);
void UpdateShaders();
int KeyAxisValue(GLFWwindow* window, int key1, int key2);
void applyJoystickInput();
bool NeedSceneUpdate();
void UpdateScene();
void RenderFrame();
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow* g_window;
int g_window_width = 960, g_window_height = 540;

Scene g_scene;
GLuint g_shader_program_scene = 0; // shader program for scene objects
GLuint g_ubo_per_frame;            // uniform buffer containing per-frame data
PerFrame g_per_frame;              // local copy of the per-frame uniform buffer

glm::vec3 g_cam_pos{4, 1.25, 4};   // camera position
glm::vec3 g_cam_velocity{0, 0, 0}; // change of g_cam_pos per tick
float g_cam_yaw = 0.785f;           // camera left-right orientation in [-pi, pi]
float g_diff_cam_yaw = 0.f;        // change of g_cam_yaw per tick
float g_cam_pitch = 0.0f;         // camera up down orientation in [-1.5, 1.5]
float g_diff_cam_pitch = 0.f;      // change of g_cam_pitch per tick

/// Entry point into the OpenGL example application.
/// @param argc Number of command line arguments.
/// @param argv argv[1] optionally names the .glb scene file to load. When it is
///             omitted, "../../scene_file/resources/scene.glb" is used - that
///             asset is not part of this repository, see README.md.
int main(int argc, char** argv) {
    try {
        // initialize glfw
        GlfwInstance::init();

        // the scene asset can be provided as the first command line argument
        std::string const scene_path =
            (argc > 1) ? std::string(argv[1]) : std::string("../../scene_file/resources/scene.glb");

        // initialize this application
        InitRenderer();
        InitScene(scene_path);

        // render until the window should close
        while (!glfwWindowShouldClose(g_window)) {
            glfwPollEvents(); // handle events
            while (NeedSceneUpdate())
                UpdateScene();         // per-tick updates
            RenderFrame();             // render image
            glfwSwapBuffers(g_window); // present image
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\nPress enter.\n";
        getchar();
        return -1;
    }
}

/// Creates the window and sets persistent render settings and compiles shaders.
/// @throw std::runtime_error If an error occurs during initialization.
void InitRenderer() {
    // create window
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    g_window = glfwCreateWindow(g_window_width, g_window_height, // initial resolution
        "SceneShading",                                          // window title
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
    if (!gladLoadGL(glfwGetProcAddress))
        throw std::runtime_error("Failed to load OpenGL context.");
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
    glGenBuffers(1, &g_ubo_per_frame);
    glBindBuffer(GL_UNIFORM_BUFFER, g_ubo_per_frame);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PerFrame), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, g_ubo_per_frame); // binding 0
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/// Creates geometry and uniform buffers.
/// @param scene_path Path of the .glb scene file that is imported. The path is
///                   resolved relative to the working directory, so it must
///                   either be absolute or the viewer must be started from the
///                   directory the relative path was written for.
void InitScene(std::string const& scene_path) {
    // load gltf file
    importGltfScene(g_scene, scene_path);

    // create pointlights
    {
        auto transform_idx = g_scene.transform_data.size();
        g_scene.transform_data.push_back(TransformComponent({
            glm::vec3(1.5, 1.7, -1.95),
            glm::quat(),
            glm::vec3(1.0),
            glm::mat4(1.0),
            NONE_INDEX,
            NONE_INDEX,
            NONE_INDEX,
        }));
        auto pointlight_idx = g_scene.pointlight_data.size();
        g_scene.pointlight_data.push_back(PointlightComponent({
            glm::vec3(1.0),
            1.0,
        }));
        g_scene.pointlights.push_back({transform_idx, pointlight_idx});
        g_per_frame.light_positions[0] = glm::vec4(g_scene.transform_data[transform_idx].position, 1.0);
        g_per_frame.light_color_intensity[0] = glm::vec4(
            g_scene.pointlight_data[pointlight_idx].light_color, g_scene.pointlight_data[pointlight_idx].intensity);
    }
    {
        auto transform_idx = g_scene.transform_data.size();
        g_scene.transform_data.push_back(TransformComponent({
            glm::vec3(0.0, 1.7, -1.95),
            glm::quat(),
            glm::vec3(1.0),
            glm::mat4(1.0),
            NONE_INDEX,
            NONE_INDEX,
            NONE_INDEX,
        }));
        auto pointlight_idx = g_scene.pointlight_data.size();
        g_scene.pointlight_data.push_back(PointlightComponent({
            glm::vec3(1.0),
            1.0,
        }));
        g_scene.pointlights.push_back({transform_idx, pointlight_idx});
        g_per_frame.light_positions[1] = glm::vec4(g_scene.transform_data[transform_idx].position, 1.0);
        g_per_frame.light_color_intensity[1] = glm::vec4(
            g_scene.pointlight_data[pointlight_idx].light_color, g_scene.pointlight_data[pointlight_idx].intensity);
    }
    {
        auto transform_idx = g_scene.transform_data.size();
        g_scene.transform_data.push_back(TransformComponent({
            glm::vec3(-1.5, 1.7, -1.95),
            glm::quat(),
            glm::vec3(1.0),
            glm::mat4(1.0),
            NONE_INDEX,
            NONE_INDEX,
            NONE_INDEX,
        }));
        auto pointlight_idx = g_scene.pointlight_data.size();
        g_scene.pointlight_data.push_back(PointlightComponent({
            glm::vec3(1.0),
            1.0,
        }));
        g_scene.pointlights.push_back({transform_idx, pointlight_idx});
        g_per_frame.light_positions[2] = glm::vec4(g_scene.transform_data[transform_idx].position, 1.0);
        g_per_frame.light_color_intensity[2] = glm::vec4(
            g_scene.pointlight_data[pointlight_idx].light_color, g_scene.pointlight_data[pointlight_idx].intensity);
    }
    {
        auto transform_idx = g_scene.transform_data.size();
        g_scene.transform_data.push_back(TransformComponent({
            glm::vec3(0.0, 2.45, 0.0),
            glm::quat(),
            glm::vec3(1.0),
            glm::mat4(1.0),
            NONE_INDEX,
            NONE_INDEX,
            NONE_INDEX,
        }));
        auto pointlight_idx = g_scene.pointlight_data.size();
        g_scene.pointlight_data.push_back(PointlightComponent({
            glm::vec3(1.0),
            2.0,
        }));
        g_scene.pointlights.push_back({transform_idx, pointlight_idx});
        g_per_frame.light_positions[3] = glm::vec4(g_scene.transform_data[transform_idx].position, 1.0);
        g_per_frame.light_color_intensity[3] = glm::vec4(
            g_scene.pointlight_data[pointlight_idx].light_color, g_scene.pointlight_data[pointlight_idx].intensity);
    }
}

MeshComponent CreateMesh(std::vector<VertexBufferDescriptor> const& vertices, IndexBufferDescriptor const& indices,
    std::vector<TextureDescriptor> const& textures) {
    MeshComponent mesh;
    mesh.index_type = indices.type;
    mesh.index_count = static_cast<int32_t>(indices.byte_size / (indices.type == GL_UNSIGNED_INT ? 4 : 2)); //assuming indices to be either 4 or 2 bytes in size

    // create, bind & fill index buffer using job.ibo to store the GL handle
    // that is, buffer the data stored in indices to a new GL_ELEMENT_ARRAY_BUFFER buffer object
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.byte_size, // data size
        indices.data,      // data pointer
        GL_STATIC_DRAW);   // usage
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // create & bind vertex array object using job.vao to store the GL handle
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    // to store non interleaved vertex data, we use a seperate vbo for each vertex attribute
    // TODO [Aufgabe 1.2]: Upload all buffers in 'vertices'.
    // Iteriere über alle Vertex-Attribute (Position, Normale, etc.)
    for (const auto& desc : vertices) {
        GLuint vbo;
        // 1. Einen neuen Vertex Buffer (VBO) generieren
        glGenBuffers(1, &vbo);
        // 2. Den Buffer an den GL_ARRAY_BUFFER binden
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // 3. Die Daten in den Buffer hochladen (statische Zeichnung)
        glBufferData(GL_ARRAY_BUFFER, desc.byte_size, desc.data, GL_STATIC_DRAW);

        // Bestimme die Shader-Location basierend auf dem semantischen Namen
        // (siehe vert_scene_objects.glsl für die Locations)
        GLuint location = 0;
        bool valid_attribute = false;

        if (desc.semantic_name == "NORMAL") {
            location = 0;
            valid_attribute = true;
        } else if (desc.semantic_name == "POSITION") {
            location = 1;
            valid_attribute = true;
        } else if (desc.semantic_name == "TANGENT") {
            location = 2;
            valid_attribute = true;
        } else if (desc.semantic_name == "TEXCOORD_0") {
            location = 3;
            valid_attribute = true;
        }

        // 4. & 5. Attribut aktivieren und Pointer setzen, falls es ein bekanntes Attribut ist
        if (valid_attribute) {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(
                location,           // Index des Attributs
                desc.size,          // Anzahl der Komponenten (z.B. 3 für vec3)
                desc.type,          // Datentyp (z.B. GL_FLOAT)
                desc.normalized,    // Ob normalisiert werden soll
                desc.stride,        // Stride (Byte-Abstand zwischen Elementen)
                (void*)desc.offset  // Offset im Buffer (hier meist 0)
            );
        }
    }

    // unbind vertex array object & vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create textures
    for (size_t tx_idx = 0; tx_idx < textures.size(); ++tx_idx) {
        GLint border = 0;
        GLuint texture_name = 0;

        // generate a new texture object using glGenTextures and store its name in texture_name
        glGenTextures(1, &texture_name);

        // bind the created texture as a 2D texture
        glBindTexture(GL_TEXTURE_2D, texture_name);

        for (auto const& tx_param : textures[tx_idx].int_parameters) {
            glTexParameteri(GL_TEXTURE_2D, tx_param.first, tx_param.second);
        }

        // use the (oldschool) function glTexImage2D to buffer the actual texture data using the
        // information and data supplied above
        glTexImage2D(GL_TEXTURE_2D, 0, textures[tx_idx].internal_format, textures[tx_idx].width,
            textures[tx_idx].height, border, textures[tx_idx].format, textures[tx_idx].type, textures[tx_idx].data);
        // generate mipmaps for the texture
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        if (textures[tx_idx].semantic_name == "Albedo") {
            mesh.albedo_tx_name = texture_name;
        } else if (textures[tx_idx].semantic_name == "Metallic_Roughness") {
            mesh.metallic_roughness_tx_name = texture_name;
        } else if (textures[tx_idx].semantic_name == "Normal") {
            mesh.normal_tx_name = texture_name;
        }
    }

    return mesh;
}

void addGltfNode(
    Scene& scene, std::shared_ptr<tinygltf::Model> const& gltf_model, int gltf_node_idx, std::size_t parent_transform_idx) {
    // add transform and load transfrom data from gltf
    auto transform_idx = scene.transform_data.size();
    scene.transform_data.push_back(TransformComponent({
        glm::vec3(0.0),
        glm::quat(),
        glm::vec3(1.0),
        glm::mat4(1.0),
        NONE_INDEX,
        NONE_INDEX,
        NONE_INDEX,
    }));

    // set local transform
    if (gltf_model->nodes[gltf_node_idx].matrix.size() != 0) // has matrix transform
    {
        glm::mat4 transformation = glm::make_mat4(gltf_model->nodes[gltf_node_idx].matrix.data()); // your transformation matrix.
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transformation, scale, rotation, translation, skew, perspective);

        scene.transform_data[transform_idx].position = translation;
        scene.transform_data[transform_idx].scale = scale;
        scene.transform_data[transform_idx].orientation = rotation;
    } else {
        auto& translation = gltf_model->nodes[gltf_node_idx].translation;
        auto& scale = gltf_model->nodes[gltf_node_idx].scale;
        auto& rotation = gltf_model->nodes[gltf_node_idx].rotation;

        if (!translation.empty()) {
            scene.transform_data[transform_idx].position = glm::vec3(static_cast<float>(translation[0]),
                static_cast<float>(translation[1]), static_cast<float>(translation[2]));
        }
        if (!scale.empty()) {
            scene.transform_data[transform_idx].scale =
                glm::vec3(static_cast<float>(scale[0]), static_cast<float>(scale[1]), static_cast<float>(scale[2]));
        }
        if (!rotation.empty()) {
            scene.transform_data[transform_idx].orientation = glm::quat(static_cast<float>(rotation[3]),
                static_cast<float>(rotation[0]), static_cast<float>(rotation[1]), static_cast<float>(rotation[2]));
        }
    }

    // set transform hierachy
    // TODO [Aufgabe 1.1]: Set parent, first_child and next_sibling of scene.transform_data[transform_idx]!
    //      parent_transform_idx is a parameter of this function.
    // Setze den Elternknoten für den aktuellen Transformationsknoten
    scene.transform_data[transform_idx].parent = parent_transform_idx;

    // Falls ein Elternknoten existiert, müssen wir den aktuellen Knoten in dessen Kinderliste einfügen
    if (parent_transform_idx != NONE_INDEX) {
        // Referenz auf den Elternknoten holen
        auto& parent_transform = scene.transform_data[parent_transform_idx];

        // Wenn der Elternknoten noch keine Kinder hat, wird der aktuelle Knoten das erste Kind
        if (parent_transform.first_child == NONE_INDEX) {
            parent_transform.first_child = transform_idx;
        } else {
            // Wenn bereits Kinder existieren, müssen wir das letzte Geschwisterkind finden
            // Starten beim ersten Kind des Elternknotens
            auto sibling_idx = parent_transform.first_child;
            
            // Solange das aktuelle Geschwisterkind einen Nachfolger hat, weitergehen
            while (scene.transform_data[sibling_idx].next_sibling != NONE_INDEX) {
                sibling_idx = scene.transform_data[sibling_idx].next_sibling;
            }
            
            // Den aktuellen Knoten als nächsten Nachbarn des letzten Geschwisterkindes setzen
            scene.transform_data[sibling_idx].next_sibling = transform_idx;
        }
    }

    // TODO [Aufgabe 1.1]: Compute transformation to world space and store it in scene.transform_data[transform_idx].world_transform.
    // Berechne die lokale Transformationsmatrix aus Position, Orientierung und Skalierung
    glm::mat4 local_transform = glm::translate(glm::mat4(1.0f), scene.transform_data[transform_idx].position) *
                                glm::toMat4(scene.transform_data[transform_idx].orientation) *
                                glm::scale(glm::mat4(1.0f), scene.transform_data[transform_idx].scale);

    // Wenn ein Elternknoten existiert, ist die Welttransformation = Welttransformation des Elternknotens * lokale Transformation
    if (parent_transform_idx != NONE_INDEX) {
        scene.transform_data[transform_idx].world_transform = scene.transform_data[parent_transform_idx].world_transform * local_transform;
    } else {
        // Ohne Elternknoten entspricht die Welttransformation der lokalen Transformation
        scene.transform_data[transform_idx].world_transform = local_transform;
    }

    // load mesh+texture data from gltf
    if (gltf_model->nodes[gltf_node_idx].mesh != -1) {
        auto primitive_cnt = gltf_model->meshes[gltf_model->nodes[gltf_node_idx].mesh].primitives.size();

        for (size_t primitive_idx = 0; primitive_idx < primitive_cnt; ++primitive_idx) {
            std::vector<VertexBufferDescriptor> vertices;
            IndexBufferDescriptor indices;
            std::vector<TextureDescriptor> textures;

            auto& indices_accessor = gltf_model->accessors[gltf_model->meshes[gltf_model->nodes[gltf_node_idx].mesh].primitives[primitive_idx].indices];
            auto& indices_bufferView = gltf_model->bufferViews[indices_accessor.bufferView];
            auto& indices_buffer = gltf_model->buffers[indices_bufferView.buffer];

            indices.data = indices_buffer.data.data() + indices_bufferView.byteOffset + indices_accessor.byteOffset;
            indices.byte_size = (indices_buffer.data.data() + indices_bufferView.byteOffset + indices_accessor.byteOffset
                + (indices_accessor.count * indices_accessor.ByteStride(indices_bufferView)))
                - (indices_buffer.data.data() + indices_bufferView.byteOffset + indices_accessor.byteOffset);
            indices.type = indices_accessor.componentType;


            auto& vertex_attributes = gltf_model->meshes[gltf_model->nodes[gltf_node_idx].mesh].primitives[primitive_idx].attributes;
            for (auto attrib : vertex_attributes) {
                auto& vertexAttrib_accessor = gltf_model->accessors[attrib.second];
                auto& vertexAttrib_bufferView = gltf_model->bufferViews[vertexAttrib_accessor.bufferView];
                auto& vertexAttrib_buffer = gltf_model->buffers[vertexAttrib_bufferView.buffer];

                // Important note!: We ignore the byte offset given by gltf because we reorder vertex data in buffers anyway
                VertexBufferDescriptor desc;
                desc.size = vertexAttrib_accessor.type;
                desc.type = vertexAttrib_accessor.componentType;
                desc.normalized = vertexAttrib_accessor.normalized;
                desc.offset = 0;
                desc.stride = vertexAttrib_accessor.ByteStride(vertexAttrib_bufferView);
                desc.semantic_name = attrib.first;

                desc.data = vertexAttrib_buffer.data.data() + vertexAttrib_bufferView.byteOffset + vertexAttrib_accessor.byteOffset;
                desc.byte_size = (vertexAttrib_buffer.data.data() + vertexAttrib_bufferView.byteOffset + vertexAttrib_accessor.byteOffset
                    + (vertexAttrib_accessor.count * vertexAttrib_accessor.ByteStride(vertexAttrib_bufferView)))
                    - (vertexAttrib_buffer.data.data() + vertexAttrib_bufferView.byteOffset + vertexAttrib_accessor.byteOffset);

                vertices.push_back(desc);
            }

            auto material_idx = gltf_model->meshes[gltf_model->nodes[gltf_node_idx].mesh].primitives[primitive_idx].material;
            if (material_idx != -1) {
                if (gltf_model->materials[material_idx].pbrMetallicRoughness.baseColorTexture.index != -1) {
                    // base color texture (diffuse albedo)
                    TextureDescriptor desc;

                    auto& img = gltf_model->images[gltf_model->textures[gltf_model->materials[material_idx].pbrMetallicRoughness.baseColorTexture.index].source];

                    desc.data = img.image.data();

                    desc.width = img.width;
                    desc.height = img.height;
                    desc.depth = 1;
                    desc.levels = 1;
                    desc.type = img.pixel_type;
                    desc.format = GL_RGBA; // apparently tinygltf enforces 4 components for better vulkan compability anyway
                    desc.internal_format = GL_RGBA8;

                    desc.int_parameters = {
                        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                        {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
                    };

                    desc.semantic_name = "Albedo";

                    textures.push_back(desc);
                }

                if (gltf_model->materials[material_idx].pbrMetallicRoughness.metallicRoughnessTexture.index != -1) {
                    // metallic roughness texture
                    TextureDescriptor desc;

                    auto& img = gltf_model->images[gltf_model->textures[gltf_model->materials[material_idx].pbrMetallicRoughness.metallicRoughnessTexture.index].source];

                    desc.data = img.image.data();

                    desc.width = img.width;
                    desc.height = img.height;
                    desc.depth = 1;
                    desc.levels = 1;
                    desc.type = img.pixel_type;
                    desc.format = GL_RGBA; // apparently tinygltf enforces 4 components for better vulkan compability anyway
                    desc.internal_format = GL_RGBA8;

                    desc.int_parameters = {
                        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                        {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
                    };

                    desc.semantic_name = "Metallic_Roughness";

                    textures.push_back(desc);
                }

                if (gltf_model->materials[material_idx].normalTexture.index != -1) {
                    // normal map texture
                    TextureDescriptor desc;

                    auto& img = gltf_model->images[gltf_model->textures[gltf_model->materials[material_idx].normalTexture.index].source];

                    desc.data = img.image.data();

                    desc.width = img.width;
                    desc.height = img.height;
                    desc.depth = 1;
                    desc.levels = 1;
                    desc.type = img.pixel_type;
                    desc.format = GL_RGBA; // 7 apparently tinygltf enforces 4 components for better vulkan compability anyway
                    desc.internal_format = GL_RGBA8;

                    desc.int_parameters = {
                        {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
                        {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
                    };

                    desc.semantic_name = "Normal";

                    textures.push_back(desc);
                }
            }

            // create render batch
            auto render_batch_idx = scene.mesh_data.size();
            scene.mesh_data.push_back(CreateMesh(vertices, indices, textures));

            // add to render objects
            scene.render_objects.push_back({transform_idx, render_batch_idx});
        }
    }

    // traverse children and add gltf nodes recursivly
    for (auto child : gltf_model->nodes[gltf_node_idx].children) {
        addGltfNode(scene, gltf_model, child, transform_idx);
    }
}

void importGltfScene(Scene& scene, std::string const& gltf_filepath) {
    // Load gltf file from disk
    std::shared_ptr<tinygltf::Model> gltf_model = std::make_shared<tinygltf::Model>();
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    auto ret = loader.LoadBinaryFromFile(gltf_model.get(), &err, &warn, gltf_filepath);

    if (!warn.empty()) {
        std::cerr << "Warn: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Err: " << err << std::endl;
    }

    if (!ret) {
        // Failing silently used to leave an empty scene on screen, which is
        // indistinguishable from a working renderer with nothing to draw.
        throw std::runtime_error(
            "Failed to load the glTF scene from \"" + gltf_filepath
            + "\". Place the scene file at that path or pass its location as the "
              "first command line argument (see README.md).");
    }

    // iterate over nodes of model and add entities+components to world
    for (auto& gltf_scene : gltf_model->scenes) {
        for (auto gltf_node : gltf_scene.nodes) {
            addGltfNode(scene, gltf_model, gltf_node, NONE_INDEX /*top level nodes have no parent*/);
        }
    }
}

/// Loads the shader programs or reloads them.
void UpdateShaders() {
    glDeleteProgram(g_shader_program_scene);

    try {
        // compile vertex & fragment shader for scene objects
        GLuint vertex_shader_scene_objs = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vertex_shader_scene_objs, "../shaders/vert_scene_objects.glsl");
        GLuint fragment_shader_scene_objs = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(fragment_shader_scene_objs, "../shaders/frag_scene_objects.glsl");

        // link shaders
        g_shader_program_scene = glCreateProgram();
        LinkProgram(g_shader_program_scene, {vertex_shader_scene_objs, fragment_shader_scene_objs});

        // clean shaders up
        glDetachShader(g_shader_program_scene, fragment_shader_scene_objs);
        glDetachShader(g_shader_program_scene, vertex_shader_scene_objs);
        glDeleteShader(fragment_shader_scene_objs);
        glDeleteShader(vertex_shader_scene_objs);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\nPress 'R' to reload the shaders.\n";
    }
}

/// Helper function for controlling an axis with 2 keys.
/// @param window The window where the keys should be pressed.
/// @param key1 The first key.
/// @param key2 The second key.
/// @return -1 if only the first key is pressed, 1 if only the second key is pressed and 0 if none
/// of the keys or both keys are pressed.
int KeyAxisValue(GLFWwindow* window, int key1, int key2) {
    bool key1_pressed = glfwGetKey(g_window, key1) == GLFW_PRESS;
    bool key2_pressed = glfwGetKey(g_window, key2) == GLFW_PRESS;
    return key2_pressed - key1_pressed;
}

double t0, t1 = 0.0;

void applyJoystickInput() {
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
    glm::vec3 vfront{0, 0, -1};
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
    glm::vec3 target_velocity = rot * glm::vec3{
                                          KeyAxisValue(g_window, GLFW_KEY_A, GLFW_KEY_D),
                                          KeyAxisValue(g_window, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE),
                                          KeyAxisValue(g_window, GLFW_KEY_W, GLFW_KEY_S),
                                      };
    g_cam_velocity = lerp(0.08f, g_cam_velocity, target_velocity);
    g_cam_pos += 0.05f * g_cam_velocity;

    // calculate view matrix and assign to g_per_frame.view
    g_per_frame.view = glm::lookAt(g_cam_pos, // camera position
        g_cam_pos + cam_dir,                  // point to look towards (not a direction)
        glm::vec3{0, 1, 0});                  // up direction

    // calculate projection matrix and assign to g_per_frame.proj
    float aspect = static_cast<float>(g_window_width) / g_window_height;
    g_per_frame.proj = glm::perspective(0.25f * glm::pi<float>(), // fov in y-direction in radians
        aspect,                                                   // aspect ratio w/h
        0.01f,                                                    // distance to near plane
        100.f);                                                   // distance to far plane
}

/// Renders a frame.
void RenderFrame() {
    // update per-frame uniform buffer
    glBindBuffer(GL_UNIFORM_BUFFER, g_ubo_per_frame);
    void* data = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(data, &g_per_frame, sizeof(g_per_frame));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // setup rendering
    glViewport(0, 0, g_window_width, g_window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render all objects
    glUseProgram(g_shader_program_scene);
    for (auto const& obj : g_scene.render_objects) {

        glUniformMatrix4fv(glGetUniformLocation(g_shader_program_scene, "model_matrix"), 1, GL_FALSE,
            glm::value_ptr(g_scene.transform_data[obj.first].world_transform));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, g_scene.mesh_data[obj.second].albedo_tx_name);
        glUniform1i(glGetUniformLocation(g_shader_program_scene, "albedo_tx2D"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, g_scene.mesh_data[obj.second].normal_tx_name);
        glUniform1i(glGetUniformLocation(g_shader_program_scene, "normal_tx2D"), 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, g_scene.mesh_data[obj.second].metallic_roughness_tx_name);
        glUniform1i(glGetUniformLocation(g_shader_program_scene, "metallic_roughness_tx2D"), 2);

        glBindVertexArray(g_scene.mesh_data[obj.second].vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_scene.mesh_data[obj.second].ibo);
        glDrawElements(GL_TRIANGLES, g_scene.mesh_data[obj.second].index_count, g_scene.mesh_data[obj.second].index_type, nullptr);
    }

    // unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/// Updates the resolution. This is called by GLFW when the resolution changes.
/// @param window The window that changed resolution.
/// @param width New width.
/// @param height New height.
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    g_window_width = width;
    g_window_height = height;
}

/// Reloads the shaders if 'R' is pressed. This is called by GLFW when a key is pressed.
/// @param window The window in which a key was pressed.
/// @param key GLFW key code of the pressed key.
/// @param scancode platform-specific key code.
/// @param action One of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
/// @param mods Modifier bits.
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        UpdateShaders();
}
