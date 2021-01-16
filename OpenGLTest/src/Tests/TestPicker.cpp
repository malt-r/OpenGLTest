#include "TestPicker.h"
#include "imgui.h"

namespace Test 
{
    TestPicker::TestPicker()
        : m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}, m_IsOpen(false), m_selectedItem(-1), m_activeTest(-1), m_currentTest(nullptr)
    {
    }

    TestPicker::~TestPicker()
    {
        if (nullptr != m_currentTest)
        {
            delete m_currentTest;
        }
    }

    void TestPicker::OnImGuiRender()
    {
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Choose a test", &m_IsOpen, ImGuiWindowFlags_MenuBar);

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
        ImGui::BeginChild("TestSelection");
        
        static const char* TestNames[]
        { 
            "ClearColor"
        };

        ImGui::ListBox((const char*)"Test:", &m_selectedItem, TestNames, IM_ARRAYSIZE(TestNames));

        if (ImGui::Button("Start test"))
        {
            StartTest();
        }

        ImGui::EndChild();

        ImGui::End();

        if (nullptr != m_currentTest)
        {
            m_currentTest->OnImGuiRender();
        }
    }

    void TestPicker::OnTestRender()
    {
        if (nullptr != m_currentTest)
        {
            m_currentTest->OnRender();
        }
    }

    void TestPicker::OnTestUpdate(float deltaTime)
    {
        if (nullptr != m_currentTest)
        {
            m_currentTest->OnUpdate(deltaTime);
        }
    }

    void TestPicker::StartTest()
    {
        if (m_activeTest != m_selectedItem)
        {
            delete m_currentTest;
            switch (m_selectedItem)
            {
            case 0:
                m_currentTest = new TestClearColor();
                break;
            default:
                break;
            }
        }
    }
}
