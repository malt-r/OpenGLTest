#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <iostream>

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error]: (" << error << ")" << 
            file << ": " << function << " (line " << line << ")" << std::endl;
        return false;
    }
    return true;
}

void GLClearError()
{
    // loop until no more errors
    while (glGetError());
}

void Renderer::Clear() const
{
    GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    //shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
