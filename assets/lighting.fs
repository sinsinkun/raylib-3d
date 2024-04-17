#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
// uniform sampler2D texture0;
// uniform vec4 colDiffuse;
in vec2 pos;

// Output fragment color
out vec4 finalColor;

void main() {
    finalColor = vec4(pos, 0.0, 1.0);;
}