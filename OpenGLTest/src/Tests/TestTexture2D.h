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
    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D();


        void OnUpdate(float deltaTime);
        void OnRender();
        void OnImGuiRender();

    private:
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;

        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;

        glm::mat4 m_Projection;
        glm::mat4 m_View;
    };
}
