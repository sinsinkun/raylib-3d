#version 330

// input vertex data from model
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// output to fragment shader
out vec3 fragColor;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
  // Send vertex attributes to fragment shader
  fragTexCoord = texCoord;
  fragColor = color;
  fragNormal = transpose(inverse(mat3(model))) * normal;

  // Calculate final vertex position
  gl_Position = projection * view * model * vec4(position, 1.0);
}