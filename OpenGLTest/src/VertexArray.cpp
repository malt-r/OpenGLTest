#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    // create and bind vertex array
    GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // bind vertexBuffer to apply operations to it
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        // actually enable attribute with index 0
        GLCall(glEnableVertexAttribArray(i));

        // specify layout of attributes (e.g. vertex position) of bound buffer
        // does not actualy store the layout to any specific buffer
        // links the index 0 of the currently bound vertex Array (vao) to the currently bound GL_ARRAY_BUFFER
        // --> different vertex buffers (GL_ARRAY_BUFFER) for different indices in one vao are possible
        GLCall(glVertexAttribPointer
        (
            i,                              // index of attribute (in the vertex itself)
            element.count,                              // number of components of the attribute (three component vector --> 3)
            element.type,                       // datatype of the component
            element.normalized,                       // normalize: should openGL normalize the value automatically?
            layout.GetStride(),                              // stride: size of the whole structure in bytes
            (const void*)offset                               // pointer: offset of attribute in the structure
        ));

        // increment offset
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
