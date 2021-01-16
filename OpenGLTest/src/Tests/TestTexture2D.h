#pragma once
#include "Test.h"

namespace Test
{
    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D();


        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();

    private:

    };
}
