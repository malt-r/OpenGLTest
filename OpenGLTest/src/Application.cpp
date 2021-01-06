#include "Glad/glad.h"
#include "GLFW/glfw3.h"

#include "iostream"

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // create empty shader object
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // set sourcecode of shader with id
    glShaderSource(id, 1, &src, nullptr);
    // compile shader
    glCompileShader(id);

    // handle error
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // sneaky way of dynamically allocate array on the stack
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    // compile vertex and fragment shader
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, vertexShader);

    // attach compiled shader-objects to program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // link all parts of the program
    glLinkProgram(program);

    // performs validation on the program and stores the result in the 'program's information log'
    // status can be queried by calling glGetProgram with arguments program and GL_VALIDATE_STATUS
    glValidateProgram(program);

    // save to delete the intermediate structures (shaders), because they were linked to a complete program
    glDeleteShader(vs);
    glDeleteShader(fs);


    return program;
}

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

    // specify layout of attributes (e.g. vertex position) of bound buffer
    glVertexAttribPointer
    (
        0,                              // index of attribute (in the vertex itself)
        2,                              // number of components of the attribute (three component vector --> 3)
        GL_FLOAT,                       // datatype of the component
        GL_FALSE,                       // normalize: should openGL normalize the value automatically?
        sizeof(float) * 2,                              // stride: size of the whole structure in bytes
        0                               // pointer: offset of attribute in the structure
    );

    // actually enable attribute with index 0
    glEnableVertexAttribArray(0);

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
