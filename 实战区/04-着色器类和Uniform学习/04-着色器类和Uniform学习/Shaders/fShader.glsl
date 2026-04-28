# version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 outPos;

uniform vec4 changeColor;

void main() {
    // FragColor = vec4(ourColor, 1.0f);
    FragColor = changeColor;
    // FragColor = vec4(outPos, 1.0f);
}