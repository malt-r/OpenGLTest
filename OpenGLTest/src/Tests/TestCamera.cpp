#include <iostream>
#include "TestCamera.h"

#include "../Renderer.h"
#include "../VertexBufferLayout.h"
#include "../Input.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

namespace Test
{
    TestCamera::TestCamera()
        :
        m_ObjPath("res/obj/cube_with_numbers.obj"),
        m_TexturePath("res/textures/numbers.png"),
        m_ModelRotationAxis(glm::vec3(1, 1, 0)),
        m_ModelRotation(glm::rotate(glm::mat4(1.0f), 0.f, m_ModelRotationAxis)),
        m_CameraController(),
        m_ModelTranslation(glm::vec3(0.f, 0.f, -10.f)),
        m_RotSpeedRadPerS(glm::quarter_pi<float>()),
        m_RotationRad(0.f),
        m_CursorDisabled(false)
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

    TestCamera::~TestCamera()
    {
    }

    void TestCamera::OnUpdate(float deltaTime)
    {
        if (Input::s_Instance()->IsMouseButtonPressed(MouseButton::Right))
        {
            Input::s_Instance()->SetMouseMode(MouseMode::Disabled);
        }
        else
        {
            Input::s_Instance()->SetMouseMode(MouseMode::Normal);
        }

        m_CameraController.OnUpdate(deltaTime);

        // animate model
        float deltaRad = m_RotSpeedRadPerS * deltaTime;
        m_RotationRad = m_RotationRad + deltaRad;
        m_ModelRotation = glm::rotate(glm::mat4(1.f), m_RotationRad, m_ModelRotationAxis);


        if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_W))
        {
            std::cout << "W pressed\n";
        }
        if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_S))
        {
            std::cout << "S pressed\n";
        }
        if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_D))
        {
            std::cout << "D pressed\n";
        }
        if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_A))
        {
            std::cout << "A pressed\n";
        }
        if (Input::s_Instance()->IsMouseButtonPressed(MouseButton::Left))
        {
            std::cout << "left mouse button pressed\n";
        }
        if (Input::s_Instance()->IsMouseButtonPressed(MouseButton::Right))
        {
            std::cout << "right mouse button pressed\n";
        }
        auto pos = Input::s_Instance()->GetMousePosition();
        std::cout << "Mouse pos: x: " << pos.x << " y:" << pos.y << std::endl;
    }

    void TestCamera::OnRender()
    {
        Renderer renderer;
        renderer.Clear();

        // calc mvp
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelTranslation) * m_ModelRotation;
        glm::mat4 pv = m_CameraController.ViewProjection();

        glm::mat4 mvp = pv * model;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture", 0);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void TestCamera::OnImGuiRender()
    {
        m_CameraController.OnImguiRender();
        ImGui::SliderFloat3("Object Translation", &m_ModelTranslation.x, -500.0f, 500.0f);
        ImGui::SliderFloat3("Rotation Axis", &m_ModelRotationAxis.x, -1.0f, 1.0f);

        ImGui::SliderFloat("Speed Speed Speed", &m_RotSpeedRadPerS, glm::quarter_pi<float>(), 10.f* glm::pi<float>());

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}