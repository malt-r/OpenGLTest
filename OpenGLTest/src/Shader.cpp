#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& filePath)
    : m_filePath(filePath), m_rendererID(0)
{
    ShaderProgramSource source = ParseShader(filePath);
    // compile shaders
    m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
}

 ShaderProgramSource Shader::ParseShader(const std::string& fileName)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, v0, v1, v2, v4));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    m_uniformLocationCache[name] = location;
    return location;
}
