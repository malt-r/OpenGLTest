#include "ObjParser.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

ObjModelData ObjParser::ReadObjectModelData(const std::string& filePath, const std::string& texturePath)
{
    std::vector<glm::vec3*> vertecies;
    std::vector<glm::vec2*> uvCoords;
    std::vector<glm::vec3*> vertexNormals;
    std::vector<std::array<ObjFaceData, 3>*> faceDataEntries;
    //std::string texture;

    std::ifstream fs(filePath);
    if (!fs)
    {
        std::cout << "Could not open " << filePath << " as ifstream" << std::endl;
    }
    else
    {
        std::string line;
        DataType type;
        while (!fs.eof())
        {
            std::getline(fs, line);
            std::stringstream lineStream(line);
            type = CheckDataType(lineStream);
            switch (type)
            {
            case DataType::vertex:
            {
                glm::vec3* vertexCoord = ParseVec3(lineStream);
                vertecies.push_back(vertexCoord);
                break;
            }
            case DataType::uvCoord:
            {
                glm::vec2* uvCoord = ParseVec2(lineStream);
                uvCoords.push_back(uvCoord);
                break;
            }
            case DataType::vertexNormal:
            {
                glm::vec3* vertexNormal = ParseVec3(lineStream);
                vertexNormals.push_back(vertexNormal);
                break;
            }
            case DataType::faceData:
            {
                std::array<ObjFaceData, 3>* faceData = ParseFaceData(lineStream); 
                faceDataEntries.push_back(faceData);
                break;
            }
            default:
                break;
            }
        }

        return { vertecies, uvCoords, vertexNormals, faceDataEntries, texturePath };
    }
}

bool ObjParser::CreateVertexAndIndexDataFromObjData
(
    const ObjModelData& modelData,
    std::vector<float>& vertexData,
    std::vector<uint32_t>& indexData
)
{
    bool bRet = true;
    // just assume, that the vectors will be empty

    //std::vector<float> vertexData;

    // problem:
    // in obj format there different uv coords for the same vertex possible,
    // so the vertex must be included with a different uv coords each time in 
    // the vertexBuffer and the indicies must match up --> generate VertexBuffer
    // and IndexBuffer in the same go

    // ignore vertex normals for now... but realistically there should not be more
    // vertex normals than vertecies, right?

    // first scan all face data for unique pairs of vertexidx and uv coord idx
    // KEY: higher dword is vertexidx, lower dword is uvIdx
    // or just use std::pair?
    std::unordered_set<uint_fast64_t> vertexUvIdxSet;
    uint_fast64_t identifier;
    for (const std::array<ObjFaceData, 3>*face : modelData.faceData)
    {
        for (const ObjFaceData& faceData : *face)
        {
            identifier = ((uint_fast64_t)faceData.vertexIdx << 32) + faceData.uvIdx;
            if (vertexUvIdxSet.find(identifier) == vertexUvIdxSet.end())
            {
                vertexUvIdxSet.insert(std::move(identifier));
            }
        }
    }

    // secondly, generate vertex-buffer entries and store the indicies of the
    // real vertecies
    uint32_t vertexIdx;
    uint32_t uvIdx;
    uint32_t idx = 0;

    // this maps the combination of the vertex idx and the uvCoord in the stored 
    // facedata to the generated vertexIdx
    std::unordered_map<uint_fast64_t, uint32_t> vertexIdxMap;
    for (const uint_fast64_t& id : vertexUvIdxSet)
    {
        vertexIdx = id >> 32;
        uvIdx = id & 0xFFFFFFFF;

        // Note: don't fuck up the vertex layout...
        vertexData.push_back(modelData.vertecies[vertexIdx]->x);
        vertexData.push_back(modelData.vertecies[vertexIdx]->y);
        vertexData.push_back(modelData.vertecies[vertexIdx]->z);
        vertexData.push_back(modelData.uvCoords[uvIdx]->x);
        vertexData.push_back(modelData.uvCoords[uvIdx]->y);

        vertexIdxMap.insert(std::pair<uint_fast64_t, uint32_t>(id, idx));
        idx++;
    }
    // TODO: make vertexBuffer

    // thirdly, iterate over faceData again and generate idxBuffer based on stored
    // real indicies

    //std::vector<uint32_t> indexData;
    for (const std::array<ObjFaceData, 3>*face : modelData.faceData)
    {
        for (const ObjFaceData& faceData : *face)
        {
            // for debugging
            uint_fast64_t identifier = ((uint_fast64_t)faceData.vertexIdx << 32) + faceData.uvIdx;
            uint32_t faceDataVertIdx = faceData.vertexIdx;
            uint32_t mappedVertIdx = vertexIdxMap[identifier];
            indexData.push_back(vertexIdxMap[identifier]);
        }
    }
    // TODO: make indexBuffer
    return bRet;
}

DataType ObjParser::CheckDataType(std::stringstream& line)
{
    std::string type;
    if (line.good())
    {
        line >> type;
        if (!strcmp(type.c_str(), "v"))
        {
            return DataType::vertex;
        }
        else if (!strcmp(type.c_str(), "vt"))
        {
            return DataType::uvCoord;
        }
        else if (!strcmp(type.c_str(), "vn"))
        {
            return DataType::vertexNormal;
        }
        else if (!strcmp(type.c_str(), "f"))
        {
            return DataType::faceData;
        }
        else
        {
            return DataType::none;
        }
    }
    return DataType::none;
}

// return smart_ptr?
glm::vec3* ObjParser::ParseVec3(std::stringstream& lineStream)
{
    std::string sf1, sf2, sf3;
    lineStream >> sf1 >> sf2 >> sf3;

    float f1 =  atof(sf1.c_str());
    float f2 =  atof(sf2.c_str());
    float f3 =  atof(sf3.c_str());

    return new glm::vec3(f1,f2,f3);
}

glm::vec2* ObjParser::ParseVec2(std::stringstream& lineStream)
{
    std::string sf1, sf2;
    lineStream >> sf1 >> sf2;

    float f1 =  atof(sf1.c_str());
    float f2 =  atof(sf2.c_str());

    return new glm::vec2(f1,f2);
}

std::array<ObjFaceData, 3>* ObjParser::ParseFaceData(std::stringstream& lineStream)
{
    std::string sf[3];
    lineStream >> sf[0] >> sf[1] >> sf[2];

    std::array<ObjFaceData, 3>* retArr = new std::array<ObjFaceData, 3>();
    std::stringstream ss;
    int idx = 0;

    uint32_t indicies[3] = {};
    uint32_t tempIdx = 0;
    for (int stringIdx = 0; stringIdx < 3; stringIdx++)
    {
        idx = 0;
        auto it = sf[stringIdx].cbegin();

        while (it != sf[stringIdx].cend())
        {
            // read char into ss
            if (*it != '/')
            {
                ss << *it;
            }

            // parse contents of ss as index
            if ('/' == *it || sf[stringIdx].cend() == it + 1)
            {
                if (idx > 2)
                {
                    std::cout << "found more than three indicies in face data, ignoring all data after position 3" << std::endl;
                }

                tempIdx = atoi(ss.str().c_str());
                // offset each index by -1 because .obj's indicies start at 1
                // but we wan't them to map directly to C++ indicies
                indicies[idx] = tempIdx - 1;

                // remove contents of ss
                ss.str(std::string());

                idx++;
            }
            it++;
        }

        // first idx: vertexidx, second idx: uvIdx, third idx: vertex normal idx
        (*retArr)[stringIdx] = ObjFaceData{ indicies[0], indicies[1], indicies[2] };
        ss.str(std::string());
    }
    return retArr;
}
