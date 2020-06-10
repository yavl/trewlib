#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoord;

uniform mat4 u_projTrans;

void main()
{
    gl_Position = u_projTrans * vec4(vertex.xy, 0.0, 1.0);
    TexCoord = vertex.zw;
}  