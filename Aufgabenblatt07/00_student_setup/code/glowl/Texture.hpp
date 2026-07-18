/*************************************************************************
* glOwl - OpenGLObjectWrapperLibrary
*------------------------------------------------------------------------
* Copyright (c) 2014-2017 Michael Becher
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <vector>

#include <glad/glad.h>

struct TextureLayout
{
    TextureLayout()
        : width(0), internal_format(0), height(0), depth(0), format(0), type(0), levels(0) {}
    /**
     * \param internal_format Specifies the (sized) internal format of a texture (e.g. GL_RGBA32F)
     * \param width Specifies the width of the texture in pixels.
     * \param height Specifies the height of the texture in pixels. Will be ignored by Texture1D.
     * \param depth Specifies the depth of the texture in pixels. Will be ignored by Texture1D and Texture2D.
     * \param format Specifies the format of the texture (e.g. GL_RGBA)
     * \param type Specifies the type of the texture (e.g. GL_FLOAT)
     */
    TextureLayout(GLint internal_format, int width, int height, int depth, GLenum format, GLenum type, GLsizei levels)
        : internal_format(internal_format), width(width), height(height), depth(depth), format(format), type(type), levels(levels) {}

    /**
    * \param internal_format Specifies the (sized) internal format of a texture (e.g. GL_RGBA32F)
    * \param width Specifies the width of the texture in pixels.
    * \param height Specifies the height of the texture in pixels. Will be ignored by Texture1D.
    * \param depth Specifies the depth of the texture in pixels. Will be ignored by Texture1D and Texture2D.
    * \param format Specifies the format of the texture (e.g. GL_RGBA)
    * \param type Specifies the type of the texture (e.g. GL_FLOAT)
    * \param int_parameters A list of integer texture parameters, each given by a pair of name and value (e.g. {{GL_TEXTURE_SPARSE_ARB,GL_TRUE},{...},...}
    * \param int_parameters A list of float texture parameters, each given by a pair of name and value (e.g. {{GL_TEXTURE_MAX_ANISOTROPY_EX,4.0f},{...},...}
    */
    TextureLayout(GLint internal_format, int width, int height, int depth, GLenum format, GLenum type, GLsizei levels, std::vector<std::pair<GLenum, GLint>> const& int_parameters, std::vector<std::pair<GLenum, GLfloat>> const& float_parameters)
        : internal_format(internal_format), width(width), height(height), depth(depth), format(format), type(type), levels(levels), int_parameters(int_parameters), float_parameters(float_parameters) {}
    TextureLayout(GLint internal_format, int width, int height, int depth, GLenum format, GLenum type, GLsizei levels, std::vector<std::pair<GLenum, GLint>>&& int_parameters, std::vector<std::pair<GLenum, GLfloat>>&& float_parameters)
        : internal_format(internal_format), width(width), height(height), depth(depth), format(format), type(type), levels(levels), int_parameters(int_parameters), float_parameters(float_parameters) {}

    GLint internal_format;
    int width;
    int height;
    int depth;
    GLenum format;
    GLenum type;

    GLsizei levels;

    std::vector<std::pair<GLenum, GLint>> int_parameters;
    std::vector<std::pair<GLenum, GLfloat>> float_parameters;
};

class Texture
{
protected:
    GLuint		m_name; ///< OpenGL texture name given by glGenTextures
    //GLuint64	m_texture_handle; ///< Actual OpenGL texture handle (used for bindless)

    GLenum		m_internal_format;
    GLenum		m_format;
    GLenum		m_type;

    GLsizei		m_levels;

    // TODO: Store texture parameters as well ?
public:
    Texture(GLint internal_format,
        GLenum format,
        GLenum type,
        GLsizei levels)
        : m_internal_format(internal_format),
        m_format(format),
        m_type(type),
        m_levels(levels) {}
    virtual ~Texture() { glDeleteTextures(1, &m_name); }
    Texture(const Texture&) = delete;

    virtual void bindTexture() const = 0;

    void bindImage(GLuint location, GLenum access) const
    {
        glBindImageTexture(location, m_name, 0, GL_TRUE, 0, access, m_internal_format);
    }

    //	void makeResident() { glMakeTextureHandleResidentARB(m_texture_handle); }
    //	void makeNonResident() { glMakeTextureHandleNonResidentARB (m_texture_handle); }

    virtual void updateMipmaps() = 0;

    virtual TextureLayout getTextureLayout() const = 0;

    GLuint getName() const { return m_name; }
    //	GLuint64 getTextureHandle() const {return m_texture_handle;}
    //	GLuint64 getImageHandle(GLint level, GLboolean layered, GLint layer) const{
    //		return glGetImageHandleARB(m_name, level, layered, layer, m_internal_format);
    //	}

    GLenum getInternalFormat() const { return m_internal_format; }
    GLenum getFormat() const { return m_format; }
    GLenum getType() const { return m_type; }
};

#endif
