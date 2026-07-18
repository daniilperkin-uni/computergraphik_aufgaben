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

#ifndef Texture2D_hpp
#define Texture2D_hpp

#include "Texture.hpp"

/**
* \class Texture2D
*
* \brief Encapsulates 2D texure functionality. Compacted and updated version of glOwl's texture classes.
*
* \author Michael Becher
*/
class Texture2D : public Texture
{
public:
    /**
    * \brief Constructor that creates and loads a 2D texture.
    *
    * \param id A identifier given to the texture object
    * \param layout A TextureLayout struct that specifies size, format and parameters for the texture
    * \param data Pointer to the actual texture data.
    * \param generateMipmap Specifies whether a mipmap will be created for the texture
    */
    Texture2D(TextureLayout const& layout, GLvoid* data, bool generateMipmap = false);
    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) = delete;
    Texture2D& operator=(const Texture2D& rhs) = delete;
    Texture2D& operator=(Texture2D&& rhs) = delete;

    /**
    * \brief Bind the texture.
    */
    void bindTexture() const;

    void updateMipmaps();

    /**
    * \brief Reload the texture with any new format, type and size.
    *
    * \param layout A TextureLayout struct that specifies size, format and parameters for the texture
    * \param data Pointer to the actual texture data.
    * \param generateMipmap Specifies whether a mipmap will be created for the texture
    */
    void reload(TextureLayout const& layout, GLvoid* data, bool generateMipmap = false);

    TextureLayout getTextureLayout() const;

    unsigned int getWidth() const;

    unsigned int getHeight() const;

private:
    unsigned int m_width;
    unsigned int m_height;
};

#endif
