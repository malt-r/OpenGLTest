#pragma once

#include <string>

#include "Test.h"
#include "../ObjParser.h"
#include "../Shader.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Texture.h"

#include <memory>

namespace Test
{
    class Test3DTexture : public Test
    {
    public:
        Test3DTexture();
        ~Test3DTexture();

        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();
    private:
        const std::string m_ObjPath;
        const std::string m_TexturePath;

        float m_Far;
        float m_Near;
        float m_FOVDeg;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<Texture> m_Texture;

        glm::vec3 m_RotationAxis;
        glm::mat4 m_Rotation;
        glm::vec3 m_Translation;
        glm::mat4 m_Projection;
        glm::vec3 m_ViewTranslation;
        glm::mat4 m_View;
        glm::vec3 m_ScaleFactors;
        glm::mat4 m_Scale;

        float m_RotationRad;
        float m_RotSpeedRadPerS;
    };
}
