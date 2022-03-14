#pragma once

#include <exception>

class TException : public std::exception
{
public:
	TException(const char* msg) : std::exception(msg) {}
};
