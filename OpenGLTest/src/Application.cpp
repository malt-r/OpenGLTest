#include "Renderer.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#define ANIMATE 0

// TODO: try modifying the vertex positions with sin or cos timebased?

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // tell glfw to use the OpenGL core profile of version 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // sync swap interval with vsync?
    glfwSwapInterval(1);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {


        const int vertecesSize = 8 * 2;
        // first vec2: x,y; second vec2: texture coordinates:
        // openGL texcoods:
        //
        // 1|
        //  |
        //  |
        //  |
        //  |
        // 0+-------------+
        //  0             1

        float verteces[vertecesSize] =
        {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        const int numIndices = 6;
        unsigned int indices[numIndices] =
        {
            0,1,2,
            2,3,0
        };

        // setup alpha blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;

        // holds vertex-data
        VertexBuffer vb(verteces, vertecesSize * sizeof(float));

        VertexBufferLayout layout;
        // creates a new vertex-attribute of two float-components
        layout.Push<float>(2);
        // texcoords
        layout.Push<float>(2);


        // adds vertex buffer with layout to vertex array
        va.AddBuffer(vb, layout);

        // create index buffer (index buffer object)
        IndexBuffer ib(indices, numIndices);

        glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        Shader shader("res/shader/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.0f, 1.0f);

        float r = 0.0f;
        float rIncrement = 0.05f;

        // currently no blending supported...
        Texture texture("res/textures/git.png");
        texture.Bind(0);
        // texture was bound to slot 0, so set corresponding uniform to 0
        shader.SetUniform1i("u_Texture", 0);

        // unbind everything
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // setup imgui
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const char* glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGui::StyleColorsDark();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // new imGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            // draw two objects in different positions with same shader, vertex array and index buffer
            // but supply different MVPs
            {
                // column-major --> multiplication from right to left to create the result we want
                // which is first apply the view matrix, then the projection matrix
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;

                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }


            // animate color
            if (r > 1.0f)
            {
                rIncrement = -0.05f;
            }
            else if (r < 0.0f)
            {
                rIncrement = 0.05f;
            }
            r += rIncrement;


           // Imgui sample window
            {
                // use memory-layout of glm::vec3 to actually pass in all members as an array here
                ImGui::SliderFloat3("objA", &translationA.x, 0.0f, 960.0f);

                ImGui::SliderFloat3("objB", &translationB.x, 0.0f, 960.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfwTerminate destroys the openGL-context
    glfwTerminate();
    // because there is no valid openGL context here, when trying to deallocate the stack allocated vertex and index buffer
    // there will be an openGL error code, which will lead to infinite loop in clear error...
    return 0;
}
