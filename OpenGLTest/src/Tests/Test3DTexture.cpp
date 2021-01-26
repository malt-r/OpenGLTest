#include "Test3DTexture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../VertexBufferLayout.h"
#include "imgui.h"

namespace Test
{
    Test3DTexture::Test3DTexture()
        :
        m_ObjPath("res/obj/cube_with_numbers.obj"),
        m_TexturePath("res/textures/numbers.png"),
        // I currently don't really understand, what is going on with the near and far
        // planes and the z-direction of openGL..
        m_Far(100.f),
        m_Near(1.f),
        m_FOVDeg(45.f),
        m_Projection(glm::perspective(glm::radians(m_FOVDeg), 960.f/540.f, m_Near, m_Far)),
        m_Translation(glm::vec3(0, 0, 200)),
        m_ViewTranslation(glm::vec3(0,0,0)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_RotationAxis(glm::vec3(1, 1, 0)),
        m_Rotation(glm::rotate(glm::mat4(1.0f), 0.f, m_RotationAxis)),
        m_ScaleFactors(glm::vec3(3.f, 3.f, 3.f)),
        m_Scale(glm::scale(glm::mat4(1.0f), m_ScaleFactors)),
        m_RotationRad(glm::quarter_pi<float>()),
        m_RotSpeedRadPerS(glm::quarter_pi<float>())
    {
        // read in obj-data and texture
        auto data = ObjParser::ReadObjectModelData(m_ObjPath, m_TexturePath);

        std::vector<float> vertexData;
        std::vector<uint32_t> indexData;
        ObjParser::CreateVertexAndIndexDataFromObjData(data, vertexData, indexData);


        m_VertexBuffer = std::make_unique<VertexBuffer>(&vertexData[0], vertexData.size() * sizeof(float));

        VertexBufferLayout layout;
        // positions
        layout.Push<float>(3);

        // texcoord
        layout.Push<float>(2);

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(&indexData[0], indexData.size());

        m_Shader = std::make_unique<Shader>("res/shader/Basic_3D_TexCoord.shader");

        m_Texture = std::make_unique<Texture>("res/textures/numbers.png");

        m_VertexBuffer->Unbind();
        m_VAO->Unbind();
        m_IndexBuffer->Unbind();
        m_Shader->Unbind();
    }

    Test3DTexture::~Test3DTexture()
    {

    }

    void Test3DTexture::OnUpdate(float deltaTime)
    {
        float deltaRad = m_RotSpeedRadPerS * deltaTime;
        m_RotationRad = m_RotationRad + deltaRad;

        m_Rotation = glm::rotate(glm::mat4(1.f), m_RotationRad, m_RotationAxis);
    }

    void Test3DTexture::OnRender()
    {
        Renderer renderer;
        renderer.Clear();

        // calc mvp
        m_Projection = glm::perspective(glm::radians(m_FOVDeg), 960.f/540.f, m_Near, m_Far);
        m_View = glm::translate(glm::mat4(1.0f), m_ViewTranslation);
        m_Scale = glm::scale(glm::mat4(1.f), m_ScaleFactors);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation) * m_Rotation * m_Scale;

        glm::mat4 mvp = m_Projection * m_View * model;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void Test3DTexture::OnImGuiRender()
    {
        ImGui::SliderFloat3("Object Translation", &m_Translation.x, -500.0f, 500.0f);
        ImGui::SliderFloat3("Object Scale", &m_ScaleFactors.x, 0.05f, 100.0f);
        ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, -1.0f, 1.0f);

        ImGui::SliderFloat("Speed Speed Speed", &m_RotSpeedRadPerS, glm::quarter_pi<float>(), 10.f* glm::pi<float>());
        ImGui::SliderFloat3("View Translation", &m_ViewTranslation.x, -500.0f, 500.0f);

        ImGui::SliderFloat("Near", &m_Near, -10.0f, 10.0f);
        ImGui::SliderFloat("Far", &m_Far, -100.0f, 1000.0f);
        ImGui::SliderFloat("FOV in Deg", &m_FOVDeg, 10.f, 89.f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}