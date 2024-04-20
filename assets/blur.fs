#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

const float blurRadius = 12.0;

// simple radial blur
vec4 radialBlur() {
  // Texel color fetching from texture sampler
  vec4 baseColor = texture(texture0, fragTexCoord);
  vec4 sum = vec4(0.0);

  float textureX = textureSize(texture0, 0).x;
  float textureY = textureSize(texture0, 0).y;
  
  // step 1: blur image by sampling neighboring pixels
  for (float offsetX = -blurRadius; offsetX <= blurRadius; offsetX += 1.0) {
    for (float offsetY = -blurRadius; offsetY <= blurRadius; offsetY += 1.0) {
      if (offsetX * offsetX + offsetY * offsetY < blurRadius * blurRadius) {
        vec2 sampleCoord = fragTexCoord + vec2(offsetX / textureX, offsetY / textureY);
        sum += texture(texture0, sampleCoord);
      }
    }
  }

  // normalize sum based on number of samples
  float numSamples = (2.0 * blurRadius + 1.0) * (blurRadius + 1.0);
  return sum / numSamples;
}

void main() {
  finalColor = radialBlur();
}
