#pragma once
#include "glad/glad.h"

//#include "VertexArray.h"
//#include "IndexBuffer.h"
//#include "Shader.h"

class VertexArray;
class IndexBuffer;
class Shader;

#define ASSERT(x) if(!(x)) __debugbreak(); // compiler intrinsic
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

bool GLLogCall(const char* function, const char* file, int line);

void GLClearError();


class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
