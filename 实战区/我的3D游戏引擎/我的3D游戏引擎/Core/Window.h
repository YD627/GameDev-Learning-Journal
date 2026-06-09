#pragma once
#ifndef Window_H
#define Window_H

struct GLFWwindow;

class Window
{
public:
	Window(int width, int height, const char* title);

	~Window();

	bool ShouldClose();

	void Update();

	GLFWwindow* GetNativeWindow();

private:
	GLFWwindow* m_Window;
};

#endif // !Window_H
