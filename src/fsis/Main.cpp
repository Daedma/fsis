#include <stdexcept>
#include <iostream>
#include "ClientApplication.hpp"

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
