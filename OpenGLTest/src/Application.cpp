#include "Glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

// TODO: try modifying the vertex positions with sin or cos timebased?

#define ASSERT(x) if(!(x)) __debugbreak(); // compiler intrinsic
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error]: (" << error << ")" << 
            file << ": " << function << " (line " << line << ")" << std::endl;
        return false;
    }
    return true;
}

static void GLClearError()
{
    // loop until no more errors
    while (glGetError());
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& fileName)
{
    std::ifstream stream(fileName);
    if (!stream)
    {
        std::cout << "could not open " << fileName << std::endl;
    }

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType mode = ShaderType::NONE;

    while (getline(stream, line))
    {
        // read lines
        if (line.find("#shader") != std::string::npos)
        {
            // select mode 
            if (line.find("vertex") != std::string::npos)
            {
                mode = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                mode = ShaderType::FRAGMENT;
            }
        }
        else
        {
            // dump line in specified string stream
            if (mode != ShaderType::NONE)
            {
                ss[(int)mode] << line << "\n";
            }
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // create empty shader object
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    // set sourcecode of shader with id
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // compile shader
    GLCall(glCompileShader(id));

    // handle error
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // sneaky way of dynamically allocate array on the stack
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());

    // compile vertex and fragment shader
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach compiled shader-objects to program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    // link all parts of the program
    GLCall(glLinkProgram(program));

    // performs validation on the program and stores the result in the 'program's information log'
    // status can be queried by calling glGetProgram with arguments program and GL_VALIDATE_STATUS
    GLCall(glValidateProgram(program));

    // save to delete the intermediate structures (shaders), because they were linked to a complete program
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

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

    // sync swap interval with vsync?
    glfwSwapInterval(1);

    // init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const int positionSize = 8;
    float positions[positionSize] =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    const int numIndices = 6;
    unsigned int indices[numIndices] =
    {
        0,1,2,
        2,3,0
    };

    // create memory buffer, store id in buffer
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));

    // 'select' buffer with specified type
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

    // specify buffer size and purpose
    GLCall(glBufferData
    (
        GL_ARRAY_BUFFER,                // type of buffer, simple vertex data in this case
        positionSize * sizeof(float),   // size of the whole buffer in bytes
        positions,                      // buffer data
        GL_STATIC_DRAW                  // use pattern --> see docs.gl
    ));

    // specify layout of attributes (e.g. vertex position) of bound buffer
    GLCall(glVertexAttribPointer
    (
        0,                              // index of attribute (in the vertex itself)
        2,                              // number of components of the attribute (three component vector --> 3)
        GL_FLOAT,                       // datatype of the component
        GL_FALSE,                       // normalize: should openGL normalize the value automatically?
        sizeof(float) * 2,                              // stride: size of the whole structure in bytes
        0                               // pointer: offset of attribute in the structure
    ));

    // actually enable attribute with index 0
    GLCall(glEnableVertexAttribArray(0));

    // create index buffer (index buffer object)
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));

    // 'select' buffer with specified type, index buffer -> element array buffer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    // specify buffer size and purpose
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,             // type of buffer, index buffer
        numIndices * sizeof(unsigned int),   // size of the whole buffer in bytes
        indices,                             // buffer data
        GL_STATIC_DRAW                       // use pattern --> see docs.gl
    ));

    ShaderProgramSource source = ParseShader("res/shader/Basic.shader");

    // compile shaders
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    // bind program
    GLCall(glUseProgram(shader));

    // set up uniform before draw call (uniforms are used per drawcall
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    // location might be -1 because the uniform is specified in shader but unused --> then openGL will strip it from code
    ASSERT(location != -1);
    // set value of uniform
    GLCall(glUniform4f(location, 0.5f, 0.0f, 0.0f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // method to draw primitives without indexbuffer
        // issues drawcall to bound buffer
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        GLCall(glUniform4f(location, r, 0.0f, 0.5f, 1.0f));
        // drawcall with index buffer
        // mode, number of indices, datatype, index buffer (bound previously, thus nullptr in this case)
        // datatype must be unsigned
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        // animate color
        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
