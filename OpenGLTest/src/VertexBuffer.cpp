#include "VertexBuffer.h"
#include "Renderer.h"

#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : m_size(size)
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
        GL_DYNAMIC_DRAW
        //GL_STATIC_DRAW                  // use pattern --> see docs.gl
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

void VertexBuffer::Update(const void* data, unsigned int size)
{
    if (size > m_size)
    {
        std::cout << "size is greate than specified size of the buffer" << std::endl;
        return;
    }

    Bind();
    // get pointer
    GLCall(void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    // now copy data into memory
    memcpy(ptr, data, size);
    // make sure to tell OpenGL we're done with the pointer
    GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}
