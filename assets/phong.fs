#version 330

struct Material {
	vec3  albedo;
	float ambience;
	float diffusivity;
	float specularity;
	float shininess;
	float bands;
};

struct pLight {
	vec3 color;
	vec3 position;
	float attL;
	float attQ;
};

struct dLight {
	vec3 color;
	vec3 direction;
	float intensity;
};

struct sLight {
	vec3 color;
	vec3 direction;
	vec3 position;
	float cutOff;
	float attL;
	float attQ;
};

// Input attributes (from vertex shader)
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec3 fragPos;

uniform vec3 viewPos;
uniform Material material;
uniform dLight light0;
uniform pLight light1;
uniform pLight light2;
uniform pLight light3;
uniform pLight light4;
uniform sLight light5;
uniform sLight light6;

// Output fragment color
out vec4 finalColor;

// directional light (NOT WORKING: ALWAYS POINTS FROM CAMERA DIRECTION)
vec3 dLightContribute(dLight light) {
	// calculate ambient color
	vec3 ambientColor = material.ambience * light.color;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(-light.direction);
	float diffuse = material.diffusivity * max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * light.color;
	// clamp diffuseColor to light bands
	if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;

	// calculate specular color
	vec3 specularColor = vec3(0.0);
	if (material.specularity > 0.0) {
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, nNormal);
		float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specularColor = material.specularity * specular * light.color;
		// clamp specularColor to light bands
		if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;
	}

	vec3 outColor = light.intensity * (ambientColor + diffuseColor + specularColor) * material.albedo;
	return outColor;
}

// point light
vec3 pLightContribute(pLight light) {
	float lightDistance = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + light.attL * lightDistance + light.attQ * lightDistance * lightDistance);
	// calculate ambient color
	vec3 ambientColor = material.ambience * light.color;

	// calculate diffused color
	vec3 nNormal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuse = material.diffusivity * max(dot(nNormal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * light.color;
	// clamp diffuseColor to light bands
	if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;
	
	// calculate specular color
	vec3 specularColor = vec3(0.0);
	if (material.specularity > 0.0) {
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, nNormal);
		float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specularColor = material.specularity * specular * light.color;
		// clamp specularColor to light bands
		if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;
	}

	vec3 outColor = attenuation * (ambientColor + diffuseColor + specularColor) * material.albedo;
	return outColor;
}

// spot light
vec3 sLightContribute(sLight light) {
	// check if light exists
	float len = length(light.color);
	if (len < 0.01) return vec3(0.0);
	// check if light is in effective cone
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	vec3 outColor = vec3(0.0);
	if (theta > light.cutOff) {
		// calculate ambient color
		vec3 ambientColor = material.ambience * light.color;

		// calculate diffused color
		vec3 nNormal = normalize(fragNormal);
		vec3 lightDir = normalize(light.position - fragPos);
		float diffuse = material.diffusivity * max(dot(nNormal, lightDir), 0.0);
		vec3 diffuseColor = diffuse * light.color;
		// clamp diffuseColor to light bands
		if (material.bands > 1.0) diffuseColor = floor(diffuseColor * material.bands)/material.bands;

		// calculate specular color
		vec3 specularColor = vec3(0.0);
		if (material.specularity > 0.0) {
			vec3 viewDir = normalize(viewPos - fragPos);
			vec3 reflectDir = reflect(-lightDir, nNormal);
			float specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
			specularColor = material.specularity * specular * light.color;
			// clamp specularColor to light bands
			if (material.bands > 1.0) specularColor = floor(specularColor * material.bands)/material.bands;
		}

		// attenuation
		float lightDistance = length(light.position - fragPos);
		float attenuation = 1.0 / (1.0 + light.attL * lightDistance + light.attQ * lightDistance * lightDistance);

		outColor = ambientColor + attenuation * (diffuseColor + specularColor) * material.albedo;
	} else {
		outColor = material.ambience * material.albedo;
	}
	return outColor;
}

void main() {
	// combine colors
	vec3 outColor = 
		dLightContribute(light0) + 
		pLightContribute(light1) + 
		pLightContribute(light2) + 
		pLightContribute(light3) +
		pLightContribute(light4) +
		sLightContribute(light5) +
		sLightContribute(light6);

	finalColor = vec4(outColor, 1.0);
}