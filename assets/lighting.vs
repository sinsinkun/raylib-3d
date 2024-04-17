#version 330

// input vertex data from model
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// output to fragment shader
out vec3 vertColor;
out vec3 vertNormal;
out vec3 vertTexCoord;

void main() {
  // Send vertex attributes to fragment shader
  vertTexCoord = texCoord;
  vertColor = color;
  vertNormal = transpose(inverse(mat3(model))) * normal;

  // Calculate final vertex position
  gl_Position = projection * view * model * vec4(position, 1.0);
}