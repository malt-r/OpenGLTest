#pragma once

#include <memory>
#include <string>
#include <array>
#include <vector>
#include "glm/glm.hpp"
#include <sstream>
#include "VertexBuffer.h"


struct ObjFaceData
{
    uint32_t vertexIdx;
    uint32_t uvIdx;
    uint32_t normalIdx;
};

// intermediate class
// use to init vertexBuffer, index buffer, etc.
struct ObjModelData
{
    std::vector<std::unique_ptr<glm::vec3>> vertecies;
    std::vector<std::unique_ptr<glm::vec2>> uvCoords;
    std::vector<std::unique_ptr<glm::vec3>> vertexNormals;
    // TODO: make this just a vector, not statically sized array
    std::vector<std::unique_ptr<std::array<ObjFaceData, 3>>> faceData;
    std::string texture;
};

enum class DataType
{
    none = -1,
    vertex = 0,
    uvCoord = 1,
    vertexNormal = 2,
    faceData = 3
};

static class ObjParser
{
public:
    static ObjModelData ReadObjectModelData(const std::string& filePath, const std::string& texturePath);
    static bool CreateVertexAndIndexDataFromObjData(const ObjModelData& modelData, std::vector<float>& vertexData, std::vector<uint32_t>& indexData);
    //static std::unique_ptr<VertexBuffer> VertexBufferFromObjData(const ObjModelData& data);

private:
    static DataType CheckDataType(std::stringstream& line);

    static std::unique_ptr<glm::vec3> ParseVec3(std::stringstream& lineStream);
    static std::unique_ptr<glm::vec2> ParseVec2(std::stringstream& lineStream);
    static std::unique_ptr<std::array<ObjFaceData, 3>> ParseFaceData(std::stringstream& lineStream);
};
