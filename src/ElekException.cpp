#include "../include/ElekException.h"
#include <sstream>

ElekException::ElekException(int line, const char* file) noexcept : line(line),file(file)
{
}

const char* ElekException::what() const noexcept
{
	std::stringstream oss;
	oss << GetType() << ":" << GetOriginString();
	whatBuffer = oss.str();
	std::wstring widestr = std::wstring(whatBuffer.begin(), whatBuffer.end());
	return whatBuffer.c_str();
}

const char* ElekException::GetType() const noexcept
{
	return "ElekException";
}

int ElekException::GetLine() const noexcept
{
	return line;
}

const std::string& ElekException::GetFile() const noexcept
{
	return file;
}

std::string ElekException::GetOriginString() const noexcept
{
	std::stringstream oss;
	oss << "[FILE] " << file << " @ [LINE] " << line;
	return oss.str();
}
