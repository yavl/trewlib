#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 u_projTrans;

void main()
{
    gl_Position = u_projTrans * vec4(position.x, position.y, position.z, 1.0);
    vertexColor = vec4(color, 1.0);
    TexCoord = vec2(texCoord.x, texCoord.y);
}