#ifndef _HEADER_H_
#define _HEADER_H_

#define GL_SILENCE_DEPRECATION
#define ASSERT(x) if (!(x)) __builtin_trap();
#define glCall(x) glClearError();\
	x;\
	ASSERT(glLogCall(#x, __FILE__, __LINE__))

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
struct	ShaderSource
{
	std::string	vertexSource {};
	std::string	fragmentSource {};
};

static void			glClearError();
static bool			glLogCall(const char *function, const char *file, int line);
ShaderSource		retrieveShader(const std::string &file_path);
static unsigned int	compileShader(unsigned int type, const std::string &source);
static unsigned int	createShader(const std::string &vertexShader, const std::string &fragmentShader);

#endif
