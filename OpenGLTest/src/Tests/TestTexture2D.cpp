#include "TestTexture2D.h"
#include "..\Renderer.h"
#include "imgui.h"

#include "..\VertexBuffer.h"
#include "..\VertexBufferLayout.h"
#include "..\glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace Test
{
    TestTexture2D::TestTexture2D()
        : 
        m_Projection(glm::ortho(0.f, 960.f, 0.f, 540.f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(200, 200, 0),
        m_TranslationB(400, 200, 0)
    {
        const int verteciesSize = 8 * 2;

        float vertecies[verteciesSize] =
        {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f
        };

        const int numIndices = 6;
        unsigned int indices[numIndices] =
        {
            0,1,2,
            2,3,0
        };

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertecies, verteciesSize * sizeof(float));

        VertexBufferLayout layout;
        // creates a new vertex-attribute of two float-components
        layout.Push<float>(2);
        // texcoords
        layout.Push<float>(2);

        // adds vertex buffer with layout to vertex array
        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // create index buffer (index buffer object)
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, numIndices);

        m_Shader = std::make_unique<Shader>("res/shader/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.5f, 0.0f, 0.0f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/git.png");

        m_VAO->Unbind();
        m_Shader->Unbind();
        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        Renderer renderer;
        renderer.Clear();

        m_Shader->Bind();
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        // draw two objects in different positions with same shader, vertex array and index buffer
        // but supply different MVPs
        {
            // column-major --> multiplication from right to left to create the result we want
            // which is first apply the view matrix, then the projection matrix
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Projection * m_View * model;

            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO.get(), *m_IndexBuffer.get(), *m_Shader.get());
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Projection * m_View * model;

            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO.get(), *m_IndexBuffer.get(), *m_Shader.get());
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("objA", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("objB", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}

