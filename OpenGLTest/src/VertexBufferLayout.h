#pragma once
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        }
        ASSERT(false);
        return 0;
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
public:
    VertexBufferLayout()
    : m_stride(0) {}

    // push new attribute to layout
    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(false);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }
    
    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        // unsure about normalization
        m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
    inline unsigned int GetStride() const { return m_stride; }

};
