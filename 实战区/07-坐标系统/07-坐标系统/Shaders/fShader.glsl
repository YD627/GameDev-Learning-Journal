# version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
uniform float awesomefaceDir;

void main() {
    // vec2 reverseXCoord = vec2(-TexCoord.x, TexCoord.y);  // ·´×ªx×ø±ê
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * awesomefaceDir, TexCoord.y)), mixValue);
}