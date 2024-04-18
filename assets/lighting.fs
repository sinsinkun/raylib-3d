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
	float diffuse = clamp(-0.1, dot(vertNormal, normalize(lightDir)), 0.8);
	vec3 diffuseColor = diffuse * lightColor * albedo;
	// combine ambient + diffused light
	vec3 outColor = ambientColor + diffuseColor;
	// clamp to shade bands
	outColor.r = outColor.r * floor(outColor.r/0.1);
	outColor.g = outColor.g * floor(outColor.g/0.1);
	outColor.b = outColor.b * floor(outColor.b/0.1);
	finalColor = vec4(outColor, 1.0);
}