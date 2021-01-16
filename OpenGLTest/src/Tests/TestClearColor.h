#pragma once
#include "Test.h"

namespace Test
{
    class TestClearColor : public Test
    {
    public:
        TestClearColor();
        ~TestClearColor();


        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();

    private:

        float m_ClearColor[4];
    };
}
