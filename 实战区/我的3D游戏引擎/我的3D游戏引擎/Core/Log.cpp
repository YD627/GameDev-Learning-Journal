#include "Log.h"
#include <iostream>
#include <ctime>
#include <sstream>

std::string Log::GetCurrentTime()
{
	time_t now = time(nullptr);

	tm localTime;

	localtime_s(&localTime, &now);

	std::stringstream ss;

	ss << localTime.tm_hour
		<< ":"
		<< localTime.tm_min
		<< ":"
		<< localTime.tm_sec;

	return ss.str();
}

void Log::Print(const std::string& level, const std::string& message)
{
	std::cout
		<< "[" << level << "]"
		<< "[" << GetCurrentTime() << "] "
		<< message
		<< std::endl;
}

void Log::Info(const std::string& message)
{
	Print("INFO", message);
}

void Log::Warn(const std::string& message)
{
	Print("WARN", message);
}

void Log::Error(const std::string& message)
{
	Print("ERROR", message);
}