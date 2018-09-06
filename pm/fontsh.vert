#version 330

layout (location = 3) in vec4 coord;

out vec2 texcoord;

uniform mat4 u_projTrans;

void main(void) {
  gl_Position = u_projTrans * vec4(coord.xy, 0, 1);
  texcoord = coord.zw;
}