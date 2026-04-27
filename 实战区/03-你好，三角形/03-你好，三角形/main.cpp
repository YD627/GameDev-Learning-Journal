# include<iostream>
# include<glad/glad.h>
# include<GLFW/glfw3.h>
using namespace std;

const int SCR_WIDTH = 800;
const int SCR_HIGHT = 600;

// 顶点着色器
// 顶点着色器的输入是顶点的位置，输出是顶点的位置。
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;"
	"void main()\n"
	"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// 片段着色器
// 片段着色器的输入是片段的颜色，输出是片段的颜色。
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char* fragment1ShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragment2ShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HIGHT, "Hello, Triangle", NULL, NULL);
	if (window == NULL) {
		cout << "��������ʧ��" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "GLAD��ʼʧ��" << endl;
		return -1;
	}

	// 编译着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// 编译片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// 链接着色器程序
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int vertexShader0 = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader0, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader1, 1, &fragment1ShaderSource, NULL);
	glShaderSource(fragmentShader2, 1, &fragment2ShaderSource, NULL);
	glCompileShader(vertexShader0);
	glCompileShader(fragmentShader1);
	glCompileShader(fragmentShader2);

	unsigned int shaderProgram1, shaderProgram2;
	shaderProgram1 = glCreateProgram();
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram1, vertexShader0);
	glAttachShader(shaderProgram1, fragmentShader1);
	glLinkProgram(shaderProgram1);
	glAttachShader(shaderProgram2, vertexShader0);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	

	// 顶点数据
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,	// 右上角
		-0.5f,  0.5f, 0.0f,	// 左上角
		 0.5f, -0.5f, 0.0f,	// 右下角

		-0.5f,  0.5f, 0.0f, // 左上角
		 0.5f, -0.5f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f	// 左下角
	};
	// 矩形顶点数据
	float rectangle[] = {
		 0.5f,  0.5f, 0.0f,	// 右上角
		-0.5f,  0.5f, 0.0f,	// 左上角
		 0.5f, -0.5f, 0.0f,	// 右下角
		-0.5f, -0.5f, 0.0f	// 左下角
	};
	// 第一个三角形
	float firstTriangle[] = {
		 0.5f,  0.5f, 0.0f,	// 右上角
		-0.5f,  0.5f, 0.0f,	// 左上角
		 0.5f, -0.5f, 0.0f,	// 右下角
	};
	// 第二个三角形
	float secondTriagnle[] = {
		-0.5f,  0.5f, 0.0f, // 左上角
		 0.5f, -0.5f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f	// 左下角
	};
	// 顶点索引数组
	unsigned int indices[] = {
		// 第一个三角形
		0, 1, 2,
		// 第二个三角形
		1, 2, 3
	};

	// 顶点缓冲区对象
	// 顶点缓冲区对象用于存储顶点数据。
	// 顶点数组对象用于存储顶点属性配置。
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// 绑定顶点缓冲区对象
	// 绑定顶点缓冲区对象后，所有对顶点缓冲区的调用都会作用于当前绑定的顶点缓冲区。
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 配置顶点属性指针
	// 配置顶点属性指针后，所有对顶点属性指针的调用都会作用于当前绑定的顶点数组对象。
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(VBO, 0);
	glBindVertexArray(0);

	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriagnle), secondTriagnle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 设置多边形的绘制模式
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绑定着色器程序
		// 绑定着色器程序后，所有对着色器程序的调用都会作用于当前绑定的着色器程序。
		glUseProgram(shaderProgram1);
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);
	glfwDestroyWindow(window);
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
}