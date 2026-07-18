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

#include "GLSLProgram.hpp"

#include <iostream>

GLSLProgram::GLSLProgram() : m_link_status(false), m_compute_shader(false), m_handle(glCreateProgram())
{
}

GLSLProgram::~GLSLProgram()
{
    glDeleteProgram(m_handle);
}


GLuint GLSLProgram::getUniformLocation(const char* name)
{
    return glGetUniformLocation(m_handle, name);
}

bool GLSLProgram::compileShaderFromString(const std::string* const source, GLenum shaderType)
{
    /* Check if the source is empty */
    if (source->empty())
    {
        m_shaderlog = "No shader source.";
        return false;
    }

    /* Create shader object */
    const GLchar* c_source = source->c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &c_source, NULL);

    if (shaderType == GL_COMPUTE_SHADER) m_compute_shader = true;

    /* Compile shader */
    glCompileShader(shader);

    /* Check for errors */
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE)
    {
        GLint logLen = 0;
        m_shaderlog = "";
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char* log = new char[logLen];
            GLsizei written;
            glGetShaderInfoLog(shader, logLen, &written, log);
            m_shaderlog = log;
            delete[] log;
        }

        glDeleteShader(shader);
        return false;
    }

    //	GLint logLen = 0;
    //	m_shaderlog = "";
    //	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    //	if(logLen > 0)
    //	{
    //		char *log = new char[logLen];
    //		GLsizei written;
    //		glGetShaderInfoLog(shader, logLen, &written, log);
    //		m_shaderlog = log;
    //		delete [] log;
    //	}

    /* Attach shader to program */
    glAttachShader(m_handle, shader);
    /* Flag shader program for deletion.
     * It will only be actually deleted after the program is deleted. (See destructor for program deletion.
     */
    glDeleteShader(shader);

    return true;
}

bool GLSLProgram::link()
{
    if (m_link_status) return true;
    glLinkProgram(m_handle);

    GLint status = GL_FALSE;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLen = 0;
        m_shaderlog.append("\n");
        glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char* log = new char[logLen];
            GLsizei written;
            glGetProgramInfoLog(m_handle, logLen, &written, log);
            m_shaderlog.append(log);
            delete[] log;
        }
        return false;
    }

    //	GLint logLen = 0;
    //	m_shaderlog = "";
    //	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
    //	if(logLen > 0)
    //	{
    //		char *log = new char[logLen];
    //		GLsizei written;
    //		glGetProgramInfoLog(m_handle, logLen, &written, log);
    //		m_shaderlog = log;
    //		delete [] log;
    //	}
    //	return false;

    m_link_status = true;
    return m_link_status;
}

bool GLSLProgram::use()
{
    if (!m_link_status) return false;

    glUseProgram(m_handle);

    return true;
}

bool GLSLProgram::dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
    GLuint current_prgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current_prgm);

    if ((current_prgm != m_handle) || !m_compute_shader)
        return false;

    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

    return true;
}

const std::string& GLSLProgram::getLog()
{
    return m_shaderlog;
}

GLuint GLSLProgram::getHandle()
{
    return m_handle;
}

bool GLSLProgram::isLinked()
{
    return m_link_status;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char* name)
{
    glBindAttribLocation(m_handle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char* name)
{
    glBindFragDataLocation(m_handle, location, name);
}

void GLSLProgram::printActiveUniforms()
{
    GLint maxLength, nUniforms;
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &nUniforms);
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    GLchar* attributeName = (GLchar*) new char[maxLength];

    GLint size, location;
    GLsizei written;
    GLenum type;

    for (int i = 0; i < nUniforms; i++)
    {
        glGetActiveUniform(m_handle, i, maxLength, &written, &size, &type, attributeName);
        location = glGetUniformLocation(m_handle, attributeName);
        std::cout << location << " - " << attributeName << "\n";
    }
    delete[] attributeName;
}

void GLSLProgram::printActiveAttributes()
{
    GLint maxLength, nAttributes;
    glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &nAttributes);
    glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

    GLchar* attributeName = (GLchar*) new char[maxLength];

    GLint written, size, location;
    GLenum type;

    for (int i = 0; i < nAttributes; i++)
    {
        glGetActiveAttrib(m_handle, i, maxLength, &written, &size, &type, attributeName);
        location = glGetAttribLocation(m_handle, attributeName);
        std::cout << location << " - " << attributeName << "\n";
    }
    delete[] attributeName;
}

void GLSLProgram::setId(const std::string& id)
{
    m_id = id;
}

std::string GLSLProgram::getId()
{
    return m_id;
}