#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position; // Vertex position
layout(location = 1) in vec4 a_Color;    // Vertex color

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color; // Use the vertex color directly
}

