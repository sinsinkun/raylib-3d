#version 330

// Input attributes (from vertex shader)
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 albedo;

// Output fragment color
out vec4 finalColor;

void main() {
	// calculate ambient color
	float ambience = 0.2;
	float attenuation = 0.8;
	// calculate diffused color
	float diffuse = max(dot(fragNormal, normalize(lightDir)), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	// combine ambient + diffused light
	vec3 outColor = attenuation * diffuseColor * albedo;
	// clamp to shade bands
	outColor = albedo * ambience + floor(outColor * 6)/ 6;
	finalColor = vec4(outColor, 1.0);
}