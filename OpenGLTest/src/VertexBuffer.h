#pragma once

class VertexBuffer
{
private :
    // OpenGL create ID for every object, that is created
    // this member will store this internal ID of OpenGL
    unsigned int m_rendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    // in an engine there would be some kind of lock mechanism for modifications of the data
};
