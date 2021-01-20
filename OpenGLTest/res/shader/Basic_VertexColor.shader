#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vertColor;

// this is called a varying --> method to get data from one shader to another
out vec4 v_interpColor;

// model view projection matrix
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_interpColor = vertColor;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_interpColor;

void main()
{
    // TODO: find better way for this...
    color = vec4(1.f, 1.f, 1.f, 1.f);
    //color = v_interpColor;
}