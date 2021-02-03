#pragma once

#include "Test.h"

#include "../ObjParser.h"
#include "../Shader.h"
#include "../VertexBuffer.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Texture.h"

#include "../Camera.h"

namespace Test
{
    class TestCamera : public Test
    {
    public:
        TestCamera();
        ~TestCamera();

        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();
    private:
        const std::string m_ObjPath;
        const std::string m_TexturePath;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<Texture> m_Texture;

        CameraController m_CameraController;

        glm::vec3 m_ModelRotationAxis;
        glm::mat4 m_ModelRotation;
        float m_RotationRad;
        float m_RotSpeedRadPerS;
        glm::vec3 m_ModelTranslation;

        bool m_CursorDisabled;
    };
}
