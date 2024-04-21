#version 330

// Input attributes (from vertex shader)
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

in vec3 fragPos;
in vec3 fragLightPos;

uniform vec3 lightColor;
uniform vec3 albedo;

// Output fragment color
out vec4 finalColor;

void main() {
	// calculate ambient color
	vec3 ambience = 0.1 * lightColor;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(fragLightPos - fragPos);
	float diffuse = max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	// clamp diffuseColor to light bands
	// diffuseColor = floor(diffuseColor * 6)/6;

	// calculate specular color
	float specularStr = 0.5;
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, nNormal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specularColor = specularStr * specular * lightColor;
	// clamp specularColor to light bands
	// specularColor = floor(specularColor * 6)/6;

	// combine colors
	vec3 outColor = (ambience + diffuseColor + specularColor) * albedo;
	finalColor = vec4(outColor, 1.0);
}