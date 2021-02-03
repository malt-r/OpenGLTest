#pragma once
#include "glm/glm.hpp"

class CameraController
{
public:
    CameraController();
    ~CameraController();

    // will handle movement
    void OnUpdate(float timestep);
    void OnImguiRender();
    inline glm::mat4 ViewProjection() { return m_ViewProjection; }
private:
    float m_TranslationSpeed;
    float m_RotationDegPerPixel;

    float m_aspectRatio;
    float m_Far;
    float m_Near;
    float m_FOVDeg;

    bool m_Look;
    float m_horizontalAngle;
    float m_verticalAngle;

    glm::vec2 m_prevMousePos;
    // View
    glm::vec3 m_ViewDirection;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_Position;

    glm::vec3 m_Translation;
    glm::mat4 m_Rotation;

    // Projection
    glm::mat4 m_Projection;

    // will be calculated every update
    glm::mat4 m_ViewProjection;
};
