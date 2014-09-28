uniform float progress;
uniform vec2 offset;

varying vec2 out_uv0;
varying vec3 oVPos;

void main() {
  vec2 position = out_uv0;
  float col = 1.0;
  float x = oVPos.x + offset.x;
  float y = oVPos.y - offset.y;
  
  float a = 3.0 * (1.0 - progress);
  float b = 2.0 * (1.0 - progress);
  
  if (x * x / (a * a) + y * y / (b * b) < 1.0) {
    col = 0.0;
  }
  
  gl_FragColor.r = 0.0;
  gl_FragColor.g = 0.0;
  gl_FragColor.b = 0.0;
  gl_FragColor.a = col;
}