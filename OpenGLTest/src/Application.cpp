#include "Glad/glad.h"
#include "GLFW/glfw3.h"

#include "iostream"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const int positionSize = 6;
    float positions[positionSize] =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    // create memory buffer, store id in buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);

    // 'select' buffer with specified type
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // specify buffer size and purpose
    glBufferData
    (
        GL_ARRAY_BUFFER,                // type of buffer, simple vertex data in this case
        positionSize * sizeof(float),   // size of the whole buffer in bytes
        positions,                      // buffer data
        GL_STATIC_DRAW                  // use pattern --> see docs.gl
    );

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // method to draw primitives without indexbuffer
        // issues drawcall to bound buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
