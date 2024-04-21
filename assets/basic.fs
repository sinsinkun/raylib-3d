#version 330

struct Material {
	vec3  albedo;
	float ambience;
	float specularity;
	float shininess;
	float bands;
};

// Input attributes (from vertex shader)
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragLightPos;

uniform Material material;
uniform vec3 lightColor;

// Output fragment color
out vec4 finalColor;

void main() {
	// calculate ambient color
	vec3 ambientColor = material.ambience * lightColor;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(fragLightPos - fragPos);
	float diffuse = max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	// clamp diffuseColor to light bands
	if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;

	// calculate specular color
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, nNormal);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = material.specularity * specular * lightColor;
	// clamp specularColor to light bands
	if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;

	// combine colors
	vec3 outColor = (ambientColor + diffuseColor + specularColor) * material.albedo;
	finalColor = vec4(outColor, 1.0);
}