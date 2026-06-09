#include "Application.h"
#include "Time.h"
#include "Log.h"
#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;

Application::Application()
{
	// 初始化 GLFW 并创建窗口
	glfwInit();

	Log::Info("GLFW initialized successfully.");

	m_Window = new Window(1280, 720, "My Engine");

	Log::Info("Window created successfully.");
}

Application::~Application()
{
	delete m_Window;

	glfwTerminate();
}

void Application::Run()
{
	while (!m_Window->ShouldClose())
	{
		Time::Update();

		m_Window->Update();
	}
}