#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main() {
  // Texel color fetching from texture sampler
  vec4 baseColor = texture(texture0, fragTexCoord);
  vec4 sum = vec4(0.0);

  const float blurRadius = 10.0;
  
  // step 1: blur image by sampling neighboring pixels
  for (float offsetX = -blurRadius; offsetX <= blurRadius; offsetX += 1.0) {
    for (float offsetY = -blurRadius; offsetY <= blurRadius; offsetY += 1.0) {
      vec2 sampleCoord = fragTexCoord + vec2(offsetX / textureSize(texture0, 0).x, offsetY / textureSize(texture0, 0).y);
      sum += texture(texture0, sampleCoord);
    }
  }

  // normalize sum based on number of samples
  float numSamples = (2.0 * blurRadius + 1.0) * (2.0 * blurRadius + 1.0);
  finalColor = sum / numSamples;
}

