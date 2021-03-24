#include "../includes/wireframe_renderer/header.h"

int main()
{
	std::cout << "Hello World" << std::endl;

	if (!glfwInit())
		std::cerr << "Initialization failed" << std::endl;

	glfwTerminate();
	return 0;
}
