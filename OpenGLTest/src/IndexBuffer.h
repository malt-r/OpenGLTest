#pragma once

class IndexBuffer
{
private :
    // OpenGL create ID for every object, that is created
    // this member will store this internal ID of OpenGL
    unsigned int m_rendererID;
    unsigned int m_count;

public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_count; }

    // in an engine there would be some kind of lock mechanism for modifications of the data
};
