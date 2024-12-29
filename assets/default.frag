#version 330 core

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;

void main()
{
	color = texture(u_texture, TexCoord) * u_color;
}