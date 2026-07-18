// Very basic scene handling

#pragma once

#include <limits>
#include <vector>

// OpenGL loader
#include <glad/gl.h>

// glm types
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

static constexpr std::size_t NONE_INDEX = std::numeric_limits<std::size_t>::max();

//// Spatial properties of an object, including
//// hierachical information to construct an
//// array-index-based scene graph
struct TransformComponent {
    /// local position stored as 3D vector
    glm::vec3 position;
    /// local orientation stored as quarternion
    glm::quat orientation;
    /// local scale stored as 3D vector
    glm::vec3 scale;

    /// global world transformation from model
    /// to world space, i.e. including hierachical
    /// transformaions
    glm::mat4 world_transform;

    /// index of parent object within transform component array
    /// a value of NONE_INDEX should indicate that object doesn't a have parent
    std::size_t parent = NONE_INDEX;
    /// index of first child object within transform component array
    /// a value of NONE_INDEX should indicate that object doesn't children
    std::size_t first_child = NONE_INDEX;
    /// index of next sibling object within transform component array
    /// allows for arbitray number of children using an index-based
    /// linked-list while each transform component retains a fixed size
    /// a value of NONE_INDEX should indicate that object doesn't have any siblings
    std::size_t next_sibling = NONE_INDEX;
};

/// Pointlight properties
struct PointlightComponent {
    /// RGB light color
    glm::vec3 light_color;
    /// light intensity (unitless)
    float intensity;
};

/// Combination of a vertex array object, an index buffer object
/// and texture names
struct MeshComponent {
    /// vertex array object
    GLuint vao;
    /// index buffer object
    GLuint ibo;
    /// number of indices
    int32_t index_count;
    /// data type of indices, e.g. GL_UNSIGNED_INT
    GLenum index_type;

    /// OpenGL texture name for albedo (surface color) map
    GLuint albedo_tx_name;
    /// OpenGL texture name for normal map
    GLuint normal_tx_name;
    /// OpenGL texture name for metallic/rougness map
    /// (usually stored in green and blue channel respectively)
    GLuint metallic_roughness_tx_name;
};

/// Scene struct that stores all scene objects and their components
struct Scene {
    /// Transform components
    std::vector<TransformComponent> transform_data;
    /// Pointlight components
    std::vector<PointlightComponent> pointlight_data;
    /// Mesh components
    std::vector<MeshComponent> mesh_data;

    /// Stores all renderable objects in the scene.
    /// Each object is made up of an index pair that references
    /// a transform component and a mesh component respectively
    std::vector<std::pair<size_t, size_t>> render_objects;
    /// Stores all pointlights in the scene.
    /// Each pointlight object is made up of an index pair that
    /// references a transform component and a pointlight
    /// component respectively
    std::vector<std::pair<size_t, size_t>> pointlights;
};
