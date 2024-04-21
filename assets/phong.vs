#version 330

// input vertex data from model
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 color;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec2 texCoord2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

// output to fragment shader
out vec3 fragColor;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec3 fragPos;
out vec3 fragLightPos;
out vec3 fragLightPos2;

void main() {
  // Send vertex attributes to fragment shader
  fragTexCoord = texCoord;
  fragColor = color;
  fragNormal = transpose(inverse(mat3(view * model))) * normal;
  fragPos = vec3(view * model * vec4(position, 1.0)); // transform world space to view space
  fragLightPos = vec3(view * vec4(lightPos, 1.0)); // transform world space to view space
  fragLightPos2 = vec3(view * vec4(lightPos.x, lightPos.y, -lightPos.z, 1.0));

  // Calculate final vertex position
  gl_Position = projection * view * model * vec4(position, 1.0);
}