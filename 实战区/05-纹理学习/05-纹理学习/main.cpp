#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

const int SCR_WIDTH = 800;
const int SCR_HIGHT = 600;
const string VSHADER_PATH = "Shaders/vShader.glsl";
const string FSHADER_PATH = "Shaders/fShader.glsl";

int main() {
	stbi_set_flip_vertically_on_load(true);	// 在调用 stbi_load之后，告诉 stb_image在加载时自动翻转Y轴。这会影响之后所有通过 stb_image加载的纹理。

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HIGHT, "Learning Textures", NULL, NULL);
	if (window == NULL) {
		cout << "创建GLFW窗口失败" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "初始化glad失败" << endl;
		return -1;
	}

	Shader ourShader(VSHADER_PATH, FSHADER_PATH);

	float vertices[] = {
		// 坐标			   颜色			    纹理坐标
		-0.5f,-0.5f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 左下角
		 0.5f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// 右下角
		-0.5f, 0.5f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// 左上角
		 0.5f, 0.5f,0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f	// 右上角
	};

	unsigned int indices[] = {
		0, 1, 2,	// 第一个三角形
		1, 2, 3		// 第二个三角形
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// --------- 纹理绑定 ----------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// 在x方向上循环包裹
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// 在y方向上循环包裹

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char* data = stbi_load("Textures/logo.jpg", &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "加载纹理失败" << endl;
	}
	stbi_image_free(data);


	while (!glfwWindowShouldClose(window)) {
		// 输入检测
		processInput(window);

		// 渲染
		glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// 检查并调用事件，交换缓冲
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}