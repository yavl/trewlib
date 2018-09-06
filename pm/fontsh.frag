#version 330

in vec2 TexCoord;

out vec4 color;

uniform sampler2D u_texture;
uniform float u_color;

void main(void) {
	color = vec4(1, 1, 1, texture2D(u_texture, TexCoord).r) * u_color;
}