#include "Camera.h"
#include "Input.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

CameraController::CameraController()
    :
    m_TranslationSpeed(5.f),
    m_RotationDegPerPixel(0.002f),
    m_aspectRatio(960.f / 540.f),
    m_Far(100.0f),
    m_Near(1.0f),
    m_FOVDeg(45.f),
    m_Projection(glm::perspective(glm::radians<float>(m_FOVDeg), m_aspectRatio, m_Near, m_Far)),
    m_Translation(glm::vec3(0.f, 0.f, 0.f)),
    m_Rotation(glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0, 1, 0))),
    m_ViewProjection(glm::mat4(1.f)),
    m_Look(false),
    m_horizontalAngle(glm::pi<float>()),
    m_verticalAngle(0.f),
    m_ViewDirection(glm::vec3(0.f, 0.f, 0.f)),
    m_Up(glm::vec3(0.f, 0.f, 0.f)),
    m_Right(glm::vec3(0.f, 0.f, 0.f)),
    m_Position(glm::vec3(0.f, 0.f, 0.f))
{
}

CameraController::~CameraController()
{
}

// TODO: this should probably never be handled here, but its just a test
void CameraController::OnUpdate(float timestep)
{
    if (Input::s_Instance()->IsMouseButtonPressed(MouseButton::Right))
    {
        if (m_Look)
        {
            auto mousePos = Input::s_Instance()->GetMousePosition();
            auto delta = m_prevMousePos - mousePos;

            // TODO: make camera rotate via mouse movement
            // Compute new orientation
            m_horizontalAngle += m_RotationDegPerPixel * delta.x;
            m_verticalAngle   += m_RotationDegPerPixel * delta.y;


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

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    // see also: https://math.libretexts.org/Bookshelves/Calculus/Book%3A_Calculus_(OpenStax)/12%3A_Vectors_in_Space/12.7%3A_Cylindrical_and_Spherical_Coordinates
    m_ViewDirection = glm::vec3
    (
        cos(m_verticalAngle) * sin(m_horizontalAngle), 
        sin(m_verticalAngle),
        cos(m_verticalAngle) * cos(m_horizontalAngle)
    );
    
    // Right vector
    m_Right = glm::vec3
    (
        sin(m_horizontalAngle - 3.14f/2.0f), 
        0, // y-axis does obviously not change
        cos(m_horizontalAngle - 3.14f/2.0f)
    );
    
    // Up vector
    m_Up = glm::cross( m_Right, m_ViewDirection );

    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_W))
    {
        // move forward
        m_Position += timestep * m_TranslationSpeed * m_ViewDirection;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_S))
    {
        // move backward
        m_Position -= timestep * m_TranslationSpeed * m_ViewDirection;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_D))
    {
        // move right
        m_Position += timestep * m_TranslationSpeed * m_Right;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_A))
    {
        // move left
        m_Position -= timestep * m_TranslationSpeed * m_Right;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_SPACE))
    {
        // move up
        m_Position += timestep * m_TranslationSpeed * m_Up;
    }
    if (Input::s_Instance()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
    {
        // move down
        m_Position -= timestep * m_TranslationSpeed * m_Up;
    }

    // equivalent to translating to m_Position and rotating into coordinate space
    // specified by direction (z), up (y) and right (x) and inverting the result
    // (because we are the camera and the world changes around us, not vice versa)
    glm::mat4 View = 
        glm::lookAt
        (
            m_Position,                     // eye: position of the camera
            m_Position + m_ViewDirection,   // center: looks here --> position plus unit direction vector
            m_Up                            // up: provides second vector to specify orientation of the camera
        );
    m_ViewProjection = m_Projection * View;
}

void CameraController::OnImguiRender()
{
    ImGui::SliderFloat("horizontal angle", &m_horizontalAngle, -glm::pi<float>(), glm::pi<float>());
    ImGui::SliderFloat("vertical angle", &m_verticalAngle, -glm::pi<float>(), glm::pi<float>());

    ImGui::Text("view direction x: %.3f y: %.3f z: %.3f", m_ViewDirection.x, m_ViewDirection.y, m_ViewDirection.z);
    ImGui::Text("up direction x: %.3f y: %.3f z: %.3f", m_Up.x, m_Up.y, m_Up.z);
    ImGui::Text("right direction x: %.3f y: %.3f z: %.3f", m_Right.x, m_Right.y, m_Right.z);
    ImGui::Text("position x: %.3f y: %.3f z: %.3f", m_Position.x, m_Position.y, m_Position.z);
}
