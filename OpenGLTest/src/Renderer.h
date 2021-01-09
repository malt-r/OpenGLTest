#pragma once
#include "glad/glad.h"

#define ASSERT(x) if(!(x)) __debugbreak(); // compiler intrinsic
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

bool GLLogCall(const char* function, const char* file, int line);

void GLClearError();
