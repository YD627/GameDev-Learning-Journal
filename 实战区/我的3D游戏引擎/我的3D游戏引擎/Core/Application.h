#pragma once
#ifndef APPLICATION_H
#define APPLICATIONN_H

#include "Window.h"

class Application
{
public:
	Application();

	~Application();

	void Run();

private:
	Window* m_Window;
};

#endif // !APPLICATION_H
