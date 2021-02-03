#pragma once
#include "glm/glm.hpp"

class CameraController
{
public:
    CameraController();
    ~CameraController();

    // will handle movement
    void OnUpdate(float timestep);
    inline glm::mat4 ViewProjection() { return m_ViewProjection; }
private:
    float m_TranslationSpeed;
    float m_RotationDegPerPixel;

    float m_aspectRatio;
    float m_Far;
    float m_Near;
    float m_FOVDeg;

    bool m_Look;

    glm::vec2 m_prevMousePos;
    // View
    glm::vec3 m_Translation;
    glm::mat4 m_Rotation;

    // Projection
    glm::mat4 m_Projection;

    // will be calculated every update
    glm::mat4 m_ViewProjection;
};
