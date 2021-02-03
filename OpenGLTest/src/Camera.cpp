#include "Camera.h"
#include "Input.h"
#include "glm/gtc/matrix_transform.hpp"

CameraController::CameraController()
    :
    m_TranslationSpeed(5.f),
    m_RotationDegPerPixel(0.2f),
    m_aspectRatio(960.f / 540.f),
    m_Far(100.0f),
    m_Near(1.0f),
    m_FOVDeg(45.f),
    m_Projection(glm::perspective(glm::radians<float>(m_FOVDeg), m_aspectRatio, m_Near, m_Far)),
    m_Translation(glm::vec3(0.f, 0.f, 0.f)),
    m_Rotation(glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0, 1, 0))),
    m_ViewProjection(glm::mat4(1.f)),
    m_Look(false)
{
}

CameraController::~CameraController()
{
}

// TODO: this should probably never be handled here, but its just a test
void CameraController::OnUpdate(float timestep)
{
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_W))
    {
        m_Translation.y += timestep * m_TranslationSpeed;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_S))
    {
        m_Translation.y -= timestep * m_TranslationSpeed;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_D))
    {
        m_Translation.x += timestep * m_TranslationSpeed;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_A))
    {
        m_Translation.x -= timestep * m_TranslationSpeed;
    }

    if (Input::s_Instance()->IsMouseButtonPressed(MouseButton::Right))
    {
        if (m_Look)
        {
            auto mousePos = Input::s_Instance()->GetMousePosition();
            auto deltaPos = mousePos - m_prevMousePos;

            // TODO: make camera rotate via mouse movement

            m_prevMousePos = mousePos;
        }
        else
        {
            m_Look = true;
            m_prevMousePos = Input::s_Instance()->GetMousePosition();
        }
    }
    else
    {
        m_Look = false;
    }


    glm::mat4 View = glm::translate(glm::mat4(1.f), m_Translation) * m_Rotation;
    m_ViewProjection = m_Projection * glm::inverse(View);
}
