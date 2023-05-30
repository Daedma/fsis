#include <new>
#include <stdexcept>
#include <iostream>
#include "ClientApplication.hpp"

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return new uint8_t[size];
	};

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return new uint8_t[size];
	};

int main(int argc, char const* argv[])
{
	try
	{
		ClientApplication app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
