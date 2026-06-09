#pragma once
#ifndef LOG_H
#define LOG_H

#include <string>

class Log
{
public:
	static void Info(const std::string& message);
	static void Warn(const std::string& message);
	static void Error(const std::string& message);

private:
	static std::string GetCurrentTime();

	static void Print(const std::string& level, const std::string& message);
};

#endif // !LOG_H
