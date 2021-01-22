#include "TestCube3D.h"
#include "glm\gtc\matrix_transform.hpp"

#include "..\VertexBufferLayout.h"

#include "imgui.h"

#include <vector>

namespace Test
{
    TestCube3D::TestCube3D()
        :
        m_aspectRatio(960.f / 540.f),
        m_Far(1.0f),
        m_Near(-1.0f),
        m_FOVDeg(45.f),
        m_Projection(glm::perspective(glm::radians<float>(m_FOVDeg), m_aspectRatio, m_Near, m_Far)),
        m_Translation(glm::vec3(0, 0, 0)),
        m_ViewTranslation(glm::vec3(0,0,0)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_RotationAxis(glm::vec3(1, 1, 0)),
        m_Rotation(glm::rotate(glm::mat4(1.0f), 0.f, m_RotationAxis)),
        m_ScaleFactors(glm::vec3(1.f, 1.f, 1.f)),
        m_Scale(glm::scale(glm::mat4(1.0f), m_ScaleFactors)),
        m_RotationRad(glm::quarter_pi<float>()),
        m_RotSpeedRadPerS(glm::quarter_pi<float>())
    {
        float positions[] =
        {
            // front side
            -0.5f, -0.5f, 0.5f,
             0.5f, -0.5f, 0.5f,
             0.5f,  0.5f, 0.5f,
            -0.5f,  0.5f, 0.5f,

            // back side
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        glm::vec4 color;
        int iter;
        for (int i = 0; i < sizeof(positions) / sizeof(float) / 3; i++)
        {
            iter = i % 2;
            if (iter == 0)
            {
                color = glm::vec4(1.f, 0.f, 0.f, 1.f);
            }
            else if (iter == 1)
            {
                color = glm::vec4(0.f, 1.f, 0.f, 1.f);
            }
            else if (iter == 2)
            {
                color = glm::vec4(0.f, 0.f, 1.f, 1.f);
            }
            else 
            {
                color = glm::vec4(0.f, 0.f, 0.f, 1.f);
            }

            m_vertecies[i].Position = { positions[i*3], positions[i*3 + 1], positions[i*3 + 2] };
            m_vertecies[i].Color = color;
        }

        m_VertexBuffer = std::make_unique<VertexBuffer>(m_vertecies, sizeof(m_vertecies));


        VertexBufferLayout layout;
        // positions
        layout.Push<float>(3);

        // vertex-color
        layout.Push<float>(4);

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        uint32_t indicies[] =
        {
            // front quad
            0, 1, 2,
            2, 3, 0,

            // right side
            1, 5, 6,
            6, 2, 1,

            // left side
            4, 0, 3,
            3, 7, 4,

            // back side
            5, 4, 7,
            7, 6, 5,

            // top side
            7, 3, 2,
            2, 6, 7,

            // bottom side
            0, 1, 5,
            5, 4, 0
        };

        m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(uint32_t));

        m_Shader = std::make_unique<Shader>("res/shader/Basic_VertexColor.shader");

        m_VertexBuffer->Unbind();
        m_VAO->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();
    }

    TestCube3D::~TestCube3D()
    {
    }

    void TestCube3D::OnUpdate(float deltaTime)
    {
        int test = sizeof(Vertex);
        //interpret deltatime as s
        float deltaRad = m_RotSpeedRadPerS * deltaTime;
        m_RotationRad = m_RotationRad + deltaRad;
        m_Rotation = glm::rotate(glm::mat4(1.f), m_RotationRad, m_RotationAxis);
    }

    void TestCube3D::OnRender()
    {
        Renderer renderer;
        renderer.Clear();

        // calc mvp
        m_Projection = glm::perspective(glm::radians(m_FOVDeg), m_aspectRatio, m_Near, m_Far);
        m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
        m_Scale = glm::scale(glm::mat4(1.f), m_ScaleFactors);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation) * m_Rotation * m_Scale;

        glm::mat4 mvp = m_Projection * m_View * model;

        for (int i = 0; i < 8; i++)
        {
            glm::vec4 vertexPos = 
                glm::vec4
                (
                    m_vertecies[i].Position.x,
                    m_vertecies[i].Position.y,
                    m_vertecies[i].Position.z,
                    1
                );
            m_vertexPosScreenSpace[i] = mvp * vertexPos;
        }

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestCube3D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Object Translation", &m_Translation.x, -500.0f, 500.0f);
        ImGui::SliderFloat3("Object Scale", &m_ScaleFactors.x, 0.05f, 10.0f);
        ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, -1.0f, 1.0f);

        ImGui::SliderFloat("Speed Speed Speed", &m_RotSpeedRadPerS, glm::quarter_pi<float>(), 10.f* glm::pi<float>());
        ImGui::SliderFloat3("View Translation", &m_ViewTranslation.x, -500.0f, 500.0f);

        ImGui::SliderFloat("Near", &m_Near, -10.0f, 10.0f);
        ImGui::SliderFloat("Far", &m_Far, 0.0f, 300.0f);
        ImGui::SliderFloat("FOV in Deg", &m_FOVDeg, 10.f, 89.f);

        ImGui::Text("Vert0: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[0].x, m_vertexPosScreenSpace[0].y, m_vertexPosScreenSpace[0].z, m_vertexPosScreenSpace[0].w);
        ImGui::Text("Vert1: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[1].x, m_vertexPosScreenSpace[1].y, m_vertexPosScreenSpace[1].z, m_vertexPosScreenSpace[1].w);
        ImGui::Text("Vert2: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[2].x, m_vertexPosScreenSpace[2].y, m_vertexPosScreenSpace[2].z, m_vertexPosScreenSpace[2].w);
        ImGui::Text("Vert3: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[3].x, m_vertexPosScreenSpace[3].y, m_vertexPosScreenSpace[3].z, m_vertexPosScreenSpace[3].w);
        ImGui::Text("Vert4: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[4].x, m_vertexPosScreenSpace[4].y, m_vertexPosScreenSpace[4].z, m_vertexPosScreenSpace[4].w);
        ImGui::Text("Vert5: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[5].x, m_vertexPosScreenSpace[5].y, m_vertexPosScreenSpace[5].z, m_vertexPosScreenSpace[5].w);
        ImGui::Text("Vert6: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[6].x, m_vertexPosScreenSpace[6].y, m_vertexPosScreenSpace[6].z, m_vertexPosScreenSpace[6].w);
        ImGui::Text("Vert7: x: %.3f, y: %.3f, z: %.3f, w: %.3f", m_vertexPosScreenSpace[7].x, m_vertexPosScreenSpace[7].y, m_vertexPosScreenSpace[7].z, m_vertexPosScreenSpace[7].w);


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    std::unique_ptr<VertexBuffer> TestCube3D::MakeCubeVertexBuffer()
    {
        float positions[] =
        {
            // front side
            -50.0f, -50.0f, -50.f,
             50.0f, -50.0f, -50.f,
             50.0f,  50.0f, -50.f,
            -50.0f,  50.0f, -50.f,

            // back side
            -50.0f, -50.0f,  50.f,
             50.0f, -50.0f,  50.f,
             50.0f,  50.0f,  50.f,
            -50.0f,  50.0f,  50.f,
        };

        Vertex vertecies[8] = {};

        glm::vec4 color;
        int iter;
        for (int i = 0; i < sizeof(positions) / sizeof(float) / 3; i++)
        {
            iter = i % 4;
            if (iter == 0)
            {
                color = glm::vec4(1.f, 0.f, 0.f, 1.f);
            }
            else if (iter == 1)
            {
                color = glm::vec4(0.f, 1.f, 0.f, 1.f);
            }
            else if (iter == 2)
            {
                color = glm::vec4(0.f, 0.f, 1.f, 1.f);
            }
            else 
            {
                color = glm::vec4(0.f, 0.f, 0.f, 1.f);
            }

            vertecies[i].Position = { positions[i*3], positions[i*3 + 1], positions[i*3 + 2] };
            vertecies[i].Color = color;
        }

        auto vb = std::make_unique<VertexBuffer>(vertecies, sizeof(vertecies));


        return vb;
    }
}
