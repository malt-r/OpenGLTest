#include "Renderer.h"

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
