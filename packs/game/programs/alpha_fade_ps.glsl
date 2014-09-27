uniform float progress;
uniform vec2 offset;

varying vec2 out_uv0;
varying vec3 oVPos;

void main() {
  float col = progress;
  
  gl_FragColor.r = 0.0;
  gl_FragColor.g = 0.0;
  gl_FragColor.b = 0.0;
  gl_FragColor.a = col;
}