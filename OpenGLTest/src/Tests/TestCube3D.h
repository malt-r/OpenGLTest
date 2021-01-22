#pragma once
#include "Test.h"
#include "glm/glm.hpp"

#include "..\Shader.h"
#include "..\IndexBuffer.h"
#include "..\VertexArray.h"
#include "..\Texture.h"
#include "..\VertexBuffer.h"

#include <memory>

namespace Test
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };

    class TestCube3D : public Test
    {
    public:
        TestCube3D();
        ~TestCube3D();

        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();

    private:
        std::unique_ptr<VertexBuffer> MakeCubeVertexBuffer();
    private:
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;

        float m_aspectRatio;
        float m_Far;
        float m_Near;
        float m_FOVDeg;

        glm::vec3 m_RotationAxis;
        glm::mat4 m_Rotation;
        glm::vec3 m_Translation;
        glm::mat4 m_Projection;
        glm::vec3 m_ViewTranslation;
        glm::mat4 m_View;
        glm::vec3 m_ScaleFactors;
        glm::mat4 m_Scale;

        Vertex m_vertecies[8] = {};
        glm::vec4 m_vertexPosScreenSpace[8] = {};

        float m_RotationRad;
        float m_RotSpeedRadPerS;
    };
}
