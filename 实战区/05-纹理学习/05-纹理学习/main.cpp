#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

const int SCR_WIDTH = 800;
const int SCR_HIGHT = 600;
const string VSHADER_PATH = "Shaders/vShader.glsl";
const string FSHADER_PATH = "Shaders/fShader.glsl";

float mixValue = 0.2f;
float awesomefaceDir = 1.0f;

int main() {
	stbi_set_flip_vertically_on_load(true);	// �ڵ��� stbi_load֮�󣬸��� stb_image�ڼ���ʱ�Զ���תY�ᡣ���Ӱ��֮������ͨ�� stb_image���ص�������

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HIGHT, "Learning Textures", NULL, NULL);
	if (window == NULL) {
		cout << "����GLFW����ʧ��" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "��ʼ��gladʧ��" << endl;
		return -1;
	}

	Shader ourShader(VSHADER_PATH, FSHADER_PATH);

	float vertices[] = {
		// ����			   ��ɫ			    ��������
		-0.5f,-0.5f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// ���½�
		 0.5f,-0.5f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// ���½�
		-0.5f, 0.5f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// ���Ͻ�
		 0.5f, 0.5f,0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f	// ���Ͻ�
	};

	unsigned int indices[] = {
		0, 1, 2,	// ��һ��������
		1, 2, 3		// �ڶ���������
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

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// --------- ������ ----------
	unsigned int texture[2];
	glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// ��x������ѭ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// ��y������ѭ������

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "��������ʧ��" << endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "加载图片数据失败" << endl;
	}
	stbi_image_free(data);

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1);
	

	while (!glfwWindowShouldClose(window)) {
		// ������
		processInput(window);

		// ��Ⱦ
		glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		ourShader.setFloat("mixValue", mixValue);
		ourShader.setFloat("awesomefaceDir", awesomefaceDir);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ��鲢�����¼�����������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.0001f;
		if (mixValue >= 1.0f) {
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.0001f;
		if (mixValue <= 0.0f) {
			mixValue = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		awesomefaceDir = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		awesomefaceDir = -1.0f;
	}
}