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

#ifndef GLSLProgram_h
#define GLSLProgram_h

#include <string>

#include <glad/glad.h>

class GLSLProgram
{
private:
    GLuint m_handle;
    bool m_link_status;
    bool m_compute_shader;
    std::string m_shaderlog;

    // TODO: this is simply a hotfix solution
    std::string m_id;

public:
    GLSLProgram();
    ~GLSLProgram();

    /* Deleted copy constructor (C++11). No going around deleting copies of OpenGL Object with identical handles! */
    GLSLProgram(const GLSLProgram& cpy) = delete;

    GLSLProgram(GLSLProgram&& other) = delete;

    GLSLProgram& operator=(const GLSLProgram& rhs) = delete;
    GLSLProgram& operator=(GLSLProgram&& rhs) = delete;

    bool compileShaderFromString(const std::string* const source, GLenum shaderType);
    bool link();
    bool use();
    bool dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
    const std::string& getLog();
    GLuint getHandle();
    GLuint getUniformLocation(const char* name);
    bool isLinked();
    void bindAttribLocation(GLuint location, const char* name);
    void bindFragDataLocation(GLuint location, const char* name);
    void printActiveUniforms();
    void printActiveAttributes();

    void setId(const std::string& id);
    std::string getId();
};

#endif
