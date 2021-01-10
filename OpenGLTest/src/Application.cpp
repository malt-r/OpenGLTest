#include "Renderer.h"
#include "GLFW/glfw3.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
    window = glfwCreateWindow(2*640, 2*480, "Hello World", NULL, NULL);
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
        const int positionSize = 8;
        float positions[positionSize] =
        {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };

        float animatedPositions[positionSize] = { 0 };

        const int numIndices = 6;
        unsigned int indices[numIndices] =
        {
            0,1,2,
            2,3,0
        };

        VertexArray va;

        // holds vertex-data
        VertexBuffer vb(positions, positionSize * sizeof(float));

        VertexBufferLayout layout;
        // creates a new vertex-attribute of two float-components
        layout.Push<float>(2);

        // adds vertex buffer with layout to vertex array
        va.AddBuffer(vb, layout);

        // create index buffer (index buffer object)
        IndexBuffer ib(indices, numIndices);
        Shader shader("res/shader/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.0f, 1.0f);

        float r = 0.0f;
        float rIncrement = 0.05f;

        float o = 0.0f;
        float oIncrement = 0.008f;

        // unbind everything
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        float sign = 1.f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            //update animatedPositions
#if ANIMATE == 1
            for (int i = 0; i < positionSize; i++)
            {
                if (i % 2 == 0)
                   sign = -sign;
                animatedPositions[i] = positions[i] + sign * o;
            }
            vb.Update(animatedPositions, positionSize * sizeof(float));
#endif

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // typical procedure before draw call
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);

            va.Bind();
            ib.Bind();

            // drawcall with index buffer
            // mode, number of indices, datatype, index buffer (bound previously, thus nullptr in this case)
            // datatype must be unsigned
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

            if (o > 0.4f || o < -0.4f )
            {
                oIncrement = -oIncrement;
            }

            o += oIncrement;




            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    // glfwTerminate destroys the openGL-context
    glfwTerminate();
    // because there is no valid openGL context here, when trying to deallocate the stack allocated vertex and index buffer
    // there will be an openGL error code, which will lead to infinite loop in clear error...
    return 0;
}
