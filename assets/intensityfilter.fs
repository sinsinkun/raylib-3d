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
  // use this for average RGB intensity filter
  // float influence = step(0.95, (baseColor.r + baseColor.g + baseColor.b) / 3);
  finalColor = step(0.95, baseColor);
}