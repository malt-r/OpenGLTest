#pragma once
#include "Test.h"
#include "TestClearColor.h"

namespace Test
{
    class TestPicker
    {
    public:
        TestPicker();
        ~TestPicker();

        void OnImGuiRender();
        void OnTestRender();
        void OnTestUpdate(float deltaTime);

    private:
        float m_ClearColor[4];
        bool m_IsOpen;
        int m_selectedItem;
        int m_activeTest;
        Test* m_currentTest;

        void StartTest();
    };
}
