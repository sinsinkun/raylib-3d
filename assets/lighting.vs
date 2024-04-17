#version 330

in vec3 vertPosition;
in vec2 vertTexCoord;

out vec2 pos;
out vec4 finalPosition;

void main() {
  pos = vertTexCoord;

  finalPosition = vec4(vertTexCoord, 0.0, 1.0);
}