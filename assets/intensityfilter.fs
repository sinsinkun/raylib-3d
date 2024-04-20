#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main() {
  // Texel color fetching from texture sampler
  vec4 baseColor = texture(texture0, fragTexCoord);

  finalColor = step(vec4(0.9), baseColor);
}