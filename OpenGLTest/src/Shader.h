#pragma once

#include <string>
#include <unordered_map>

#include "Renderer.h"
#include "glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int m_rendererID;
    std::string m_filePath;

    std::unordered_map<std::string, int> m_uniformLocationCache;

public:
    Shader(const std::string& filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    ShaderProgramSource ParseShader(const std::string& fileName);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    void SetUniform1i(const std::string& name, int i);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    int GetUniformLocation(const std::string& name);
};
