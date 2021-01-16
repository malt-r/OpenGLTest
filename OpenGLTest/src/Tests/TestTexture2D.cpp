#include "TestTexture2D.h"
#include "..\Renderer.h"
#include "imgui.h"

namespace Test
{
    TestTexture2D::TestTexture2D()
    {
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestTexture2D::OnImGuiRender()
    {
    }
}

