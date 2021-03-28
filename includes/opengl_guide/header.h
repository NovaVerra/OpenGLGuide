#ifndef _HEADER_H_
#define _HEADER_H_

#define GL_SILENCE_DEPRECATION

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

ShaderSource		retrieveShader(const std::string &file_path);
static unsigned int	compileShader(unsigned int type, const std::string &source);
static unsigned int	createShader(const std::string &vertexShader, const std::string &fragmentShader);

#endif
