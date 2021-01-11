#pragma once
#include "Renderer.h"

#include <string>

class Texture
{
private:
    unsigned int m_rendererID;
    std::string m_FilePath;
    unsigned char* m_localBuffer;
    // BPP = bits per pixel
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::string& path);
    ~Texture();

    // optional slot parameter specifies the texture slot for binding multiple textures
    // typically max 32
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

};
