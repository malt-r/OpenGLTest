#include "Texture.h"

#include "stb_image.h"

Texture::Texture(const std::string& path)
    :
    m_rendererID(0),
    m_FilePath(path),
    m_localBuffer(nullptr),
    m_Height(0),
    m_Width(0),
    m_BPP(0)
{
    // openGL sets texcoord (0,0) on bottom left of texture
    // stbi loads png as scanlines, therefore we need to flip the png vertically
    stbi_set_flip_vertically_on_load(1);

    // desired channels = 4, because RGBA
    m_localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_rendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

    // MINIMAL DEFAULTS
    // specify minification filter, defines, how the texture will be 
    // sampled down, if it has to be
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // wrap modes s/t --> like x/y
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // type, multilayered texture?, GL_RGBA --> how should OpenGL store the data plus bits per channel
    GLCall(glTexImage2D
    (
        GL_TEXTURE_2D,                // type
        0,                            // multilayered texture?
        GL_RGBA8,                     // format, in which openGL stores the texture data; 8-> bits per channel
        m_Width,
        m_Height,
        0,                            // border
        GL_RGBA,                      // format, in which we pass the data to openGL
        GL_UNSIGNED_BYTE,             // type of the data
        m_localBuffer                 // actual data
        ));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localBuffer)
    {
        stbi_image_free(m_localBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture::Bind(unsigned int slot) const
{
    ASSERT(slot < 32);
    // up to 32; definition of textureslots are consecutive numbers
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
