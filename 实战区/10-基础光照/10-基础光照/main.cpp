#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/shader_s.h"
#include "include/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastx = SCR_WIDTH / 2.0f;
float lasty = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Create a linghting scene", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return - 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and complie our shader program
	Shader objectShader("Shaders/vShader.glsl", "Shaders/fShader.glsl");
	Shader lightCubeShader("Shaders/light_vshader.glsl", "Shaders/light_fshader.glsl");

	// set up vertex data and configure vertex attributes
	float vertices[] = {
		// vertices pos	     vertices normal
		-0.5f, -0.5f, -0.5f,  0, 0, -1.0f,
		 0.5f, -0.5f, -0.5f,  0, 0, -1.0f,
		 0.5f,  0.5f, -0.5f,  0, 0, -1.0f,
		 0.5f,  0.5f, -0.5f,  0, 0, -1.0f,
		-0.5f,  0.5f, -0.5f,  0, 0, -1.0f,
		-0.5f, -0.5f, -0.5f,  0, 0, -1.0f,

		-0.5f, -0.5f,  0.5f,  0, 0,  1.0f,
		 0.5f, -0.5f,  0.5f,  0, 0,  1.0f,
		 0.5f,  0.5f,  0.5f,  0, 0,  1.0f,
		 0.5f,  0.5f,  0.5f,  0, 0,  1.0f,
		-0.5f,  0.5f,  0.5f,  0, 0,  1.0f,
		-0.5f, -0.5f,  0.5f,  0, 0,  1.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f, 0, 0,
		-0.5f,  0.5f, -0.5f,  -1.0f, 0, 0,
		-0.5f, -0.5f, -0.5f,  -1.0f, 0, 0,
		-0.5f, -0.5f, -0.5f,  -1.0f, 0, 0,
		-0.5f, -0.5f,  0.5f,  -1.0f, 0, 0,
		-0.5f,  0.5f,  0.5f,  -1.0f, 0, 0,

		 0.5f,  0.5f,  0.5f,   1.0f, 0, 0,
		 0.5f,  0.5f, -0.5f,   1.0f, 0, 0,
		 0.5f, -0.5f, -0.5f,   1.0f, 0, 0,
		 0.5f, -0.5f, -0.5f,   1.0f, 0, 0,
		 0.5f, -0.5f,  0.5f,   1.0f, 0, 0,
		 0.5f,  0.5f,  0.5f,   1.0f, 0, 0,

		-0.5f, -0.5f, -0.5f,  0, -1.0f, 0,
		 0.5f, -0.5f, -0.5f,  0, -1.0f, 0,
		 0.5f, -0.5f,  0.5f,  0, -1.0f, 0,
		 0.5f, -0.5f,  0.5f,  0, -1.0f, 0,
		-0.5f, -0.5f,  0.5f,  0, -1.0f, 0,
		-0.5f, -0.5f, -0.5f,  0, -1.0f, 0,

		-0.5f,  0.5f, -0.5f,  0,  1.0f, 0,
		 0.5f,  0.5f, -0.5f,  0,  1.0f, 0,
		 0.5f,  0.5f,  0.5f,  0,  1.0f, 0,
		 0.5f,  0.5f,  0.5f,  0,  1.0f, 0,
		-0.5f,  0.5f,  0.5f,  0,  1.0f, 0,
		-0.5f,  0.5f, -0.5f,  0,  1.0f, 0,
	};
	// first configure the cube's VA) and VBO
	unsigned int cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)) {
		// pre-frame time logic
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// input
		processInput(window);

		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// change the light's position values over time
		lightPos.x = sin(glfwGetTime()) * 2.0f;
		lightPos.y = cos(glfwGetTime()) * 2.0f;

		// be sure to activate shader when setting uniforms/drawing objects
		objectShader.use();
		objectShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		objectShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		objectShader.setVec3("lightPos", lightPos);
		objectShader.setVec3("viewPos", camera.Position);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatirx();
		objectShader.setMat4("projection", projection);
		objectShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		objectShader.setMat4("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1,&cubeVAO);
	glDeleteVertexArrays(1,&lightCubeVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos;
	lastx = xpos;
	lasty = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.ProcessMouseScroll(ypos);
}