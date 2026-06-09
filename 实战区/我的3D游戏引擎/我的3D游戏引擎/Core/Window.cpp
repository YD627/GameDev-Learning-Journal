#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

// 构造函数：初始化 GLFW 并创建窗口
Window::Window(int width, int height, const char* title)
{
	// 初始化 GLFW 并创建窗口
	glfwInit();
	m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_Window == NULL)
	{
		glfwTerminate();
	}
	else
	{
		glfwMakeContextCurrent(m_Window);
	}
}

// 析构函数：销毁窗口并终止 GLFW
Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

// 检查窗口是否应该关闭
bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

// 更新窗口：交换缓冲区并处理事件
void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

// 获取原生窗口指针
GLFWwindow* Window::GetNativeWindow()
{
	return m_Window;
}