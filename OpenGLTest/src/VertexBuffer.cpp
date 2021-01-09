#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_rendererID));

    // 'select' buffer with specified type
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));

    // specify buffer size and purpose
    GLCall(glBufferData
    (
        GL_ARRAY_BUFFER,                // type of buffer, simple vertex data in this case
        size,   // size of the whole buffer in bytes
        data,                      // buffer data
        GL_STATIC_DRAW                  // use pattern --> see docs.gl
    ));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
