# version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main() {
	// float ratio = 1.00 / 1.52; // 空气折射率
	vec3 I = normalize(Position - cameraPos); // 计算视线向量
	vec3 R = reflect(I, normalize(Normal)); // 计算反射向量
	//vec3 R = refract(I, normalize(Normal), ratio); // 计算折射向量
	FragColor = vec4(texture(skybox, R).rgb, 1.0); // 从天空盒贴图中采样颜色
}