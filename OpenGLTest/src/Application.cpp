#include "Renderer.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Input.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Tests/TestClearColor.h"
#include "Tests/TestTexture2D.h"
#include "Tests/TestCube3D.h"
#include "Tests/Test3DTexture.h"
#include "Tests/TestCamera.h"

#include <iostream>
#include <chrono>

struct AllocationMetrics
{
    uint32_t TotalAllocated;
    uint32_t TotalFreed;

    uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }

};

static AllocationMetrics s_AllocationMetrics;

void PrintMemoryUsage() 
{ 
    std::cout << "Currently using " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

void* operator new (size_t size)
{
    s_AllocationMetrics.TotalAllocated += size;
    return malloc(size);
}

void operator delete (void* memory, size_t size)
{
    s_AllocationMetrics.TotalFreed += size;
    free(memory);
}

// TODO: try modifying the vertex positions with sin or cos timebased?

int main(void)
{
    static GLFWwindow* s_ApplicationWindow;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // tell glfw to use the OpenGL core profile of version 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    s_ApplicationWindow = glfwCreateWindow(2*960, 2*540, "Hello World", NULL, NULL);
    if (!s_ApplicationWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(s_ApplicationWindow);

    // sync swap interval with vsync?
    glfwSwapInterval(1);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {
        // setup alpha blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(GL_LESS));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        // setup imgui
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(s_ApplicationWindow, true);
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGui::StyleColorsDark();

        Test::Test* currentTest = nullptr;
        Test::TestMenu* testMenu = new Test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<Test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<Test::TestTexture2D>("Texture 2D");
        testMenu->RegisterTest<Test::TestCube3D>("Cube 3D");
        testMenu->RegisterTest<Test::Test3DTexture>("3D Texture");
        testMenu->RegisterTest<Test::TestCamera>("Cameracontroller");

        using clock = std::chrono::high_resolution_clock;
        auto time_start = clock::now();

        // init input
        Input::s_Instance()->SetGLFWwindow(s_ApplicationWindow);

        while (!glfwWindowShouldClose(s_ApplicationWindow))
        {
            auto delta_time = clock::now() - time_start;
            time_start = clock::now();
            renderer.Clear();

            // new imGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (nullptr != currentTest)
            {
                currentTest->OnUpdate(delta_time.count() / (1000.f *1000.f*1000.f));
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::Text("Bytes in usage %u", s_AllocationMetrics.CurrentUsage());
                ImGui::End();
            }

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(s_ApplicationWindow, &display_w, &display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(s_ApplicationWindow);
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfwTerminate destroys the openGL-context
    glfwTerminate();
    // because there is no valid openGL context here, when trying to deallocate the stack allocated vertex and index buffer
    // there will be an openGL error code, which will lead to infinite loop in clear error...
    return 0;
}
