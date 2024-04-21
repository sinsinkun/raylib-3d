#version 330

// Input attributes (from vertex shader)
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 albedo;
uniform vec3 viewPos;

// Output fragment color
out vec4 finalColor;

void main() {
	// calculate ambient color
	vec3 ambience = vec3(0.2);

	// calculate diffused color
	float attenuation = 0.8;
	float diffuse = max(dot(fragNormal, normalize(lightDir)), 0.0);
	vec3 diffuseColor = attenuation * diffuse * lightColor;
	// clamp diffuseColor to light bands
	diffuseColor = floor(diffuseColor * 6)/6;

	// combine colors
	vec3 outColor = (ambience + diffuseColor) * albedo;
	finalColor = vec4(outColor, 1.0);
}