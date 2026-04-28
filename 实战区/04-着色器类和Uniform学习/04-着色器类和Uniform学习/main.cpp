#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"

using namespace std;

const string XOFFSET = "xOffset";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "屏幕创建失败" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "glad初始化失败" << endl;
		return -1;
	}

	Shader ourshader("Shaders/vShader.glsl", "Shaders/fShader.glsl");

	float vertices[] = {
		// 位置				颜色
		-0.5f, -0.5f,0.0f,1.0f,0.0f,0.0f,	// 左下角
		0.5f, -0.5f,0.0f, 0.0f,1.0f,0.0f,	// 右下角
		0.0f, 0.5f, 0.0f, 0.0f,0.0f,1.0f	// 顶点
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);

	float xOffset = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		// 用户输入程序
		processInput(window);

		// 系统颜色
		glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ourshader.use();	// 启动着色器程序

		float timeValue = glfwGetTime();
		float redValue = cos(timeValue) / 2.0 + 0.5;
		float greenValue = sin(timeValue) / 2.0 + 0.5f;
		int vertexColorLocation = glGetUniformLocation(ourshader.ID, "changeColor");
		if (vertexColorLocation != -1) {
			glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
		}
		/*int vertexColorLocation = glGetUniformLocation(ourshader.ID, "xOffset");
		if (vertexColorLocation != -1) {
			 glUniform1f(vertexColorLocation, -0.5f);
		}*/
		// ourshader.setFloat(XOFFSET, 0.0f);

		glBindVertexArray(VAO);	// 绑定VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 用户事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}