#version 330

struct Material {
	vec3  albedo;
	float ambience;
	float diffusivity;
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
in vec3 fragLightPos2;

uniform Material material;
uniform vec3 lightColor;

// Output fragment color
out vec4 finalColor;

// directional light (NOT WORKING: ALWAYS POINTS FROM CAMERA DIRECTION)
vec3 dLightContribute(vec3 lightColor, vec3 lightD, float intensity) {
	// calculate ambient color
	vec3 ambientColor = material.ambience * lightColor;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(-lightD);
	float diffuse = material.diffusivity * max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	// clamp diffuseColor to light bands
	if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;

	// calculate specular color
	vec3 specularColor = vec3(0.0);
	if (material.specularity > 0.0) {
		vec3 viewDir = normalize(-fragPos);
		vec3 reflectDir = reflect(-lightDir, nNormal);
		float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specularColor = material.specularity * specular * lightColor;
		// clamp specularColor to light bands
		if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;
	}

	vec3 outColor = intensity * (ambientColor + diffuseColor) * material.albedo;
	return outColor;
}

// point light
vec3 pLightContribute(vec3 lightColor, vec3 lightPos, float attl, float attq) {
	float lightDistance = length(lightPos - fragPos);
	float attenuation = 1.0 / (1.0 + attl * lightDistance + attq * lightDistance * lightDistance);
	// calculate ambient color
	vec3 ambientColor = material.ambience * lightColor;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diffuse = material.diffusivity * max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	// clamp diffuseColor to light bands
	if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;
	
	// calculate specular color
	vec3 specularColor = vec3(0.0);
	if (material.specularity > 0.0) {
		vec3 viewDir = normalize(-fragPos);
		vec3 reflectDir = reflect(-lightDir, nNormal);
		float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specularColor = material.specularity * specular * lightColor;
		// clamp specularColor to light bands
		if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;
	}

	vec3 outColor = attenuation * (ambientColor + diffuseColor + specularColor) * material.albedo;
	return outColor;
}

void main() {
	// combine colors
	vec3 outColor = pLightContribute(lightColor, fragLightPos, 0.2, 0.2);
	outColor += pLightContribute(lightColor, fragLightPos2, 0.2, 0.2);
	finalColor = vec4(outColor, 1.0);
}