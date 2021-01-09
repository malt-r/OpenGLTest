#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_rendererID));

    // 'select' buffer with specified type
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));

    // specify buffer size and purpose
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,                // type of buffer, simple vertex data in this case
        count * sizeof(unsigned int),   // size of the whole buffer in bytes
        data,                      // buffer data
        GL_STATIC_DRAW                  // use pattern --> see docs.gl
    ));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
