#version 330

// Input attributes (from vertex shader)
in vec3 vertColor;
in vec3 vertNormal;
in vec3 vertTexCoord;

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 albedo;

// Output fragment color
out vec4 finalColor;

void main() {
	// calculate ambient color
	vec3 ambientColor = 0.2 * albedo;
	// calculate diffused color
	float diffuse = max(dot(vertNormal, normalize(lightDir)), 0.0);
	vec3 diffuseColor = albedo * lightColor * diffuse;
	// combine ambient + diffused light
	vec3 outColor = ambientColor + diffuseColor;
	finalColor = vec4(outColor, 1.0);
}