#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D font_tex;
void main() {
  float alpha = texture(font_tex, uv).r;
  color = vec4(1.0, 1.0, 1.0, alpha);
}
