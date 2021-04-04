#include "../includes/opengl_guide/Shader.h"
#include "../includes/opengl_guide/Renderer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &file_path)
: file_path {file_path}, m_renderer_id {0}
{
	ShaderSource	source = retrieve_shader(file_path);
	m_renderer_id = create_shader(source.vertexSource, source.fragmentSource);

}

Shader::~Shader()
{
	glDeleteProgram(m_renderer_id);
}

ShaderSource	Shader::retrieve_shader(const std::string &file_path)
{
	std::ifstream		shaders {file_path};
	std::ostringstream	oss[2];
	std::string			line {};

	if (!shaders)
		std::cerr << "Error opening file" << std::endl;

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	while (std::getline(shaders, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
			else
				;
		}
		else
			oss[(int)type] << line << '\n';
	}
	return {oss[0].str(), oss[1].str()};
}

unsigned int	Shader::compile_shader(unsigned int type, const std::string &source)
{
	unsigned int	id {glCreateShader(type)};
	const char		*src {source.c_str()};
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int				result;	
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int		length {};
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char	*message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader" << std::endl;
		std::cerr << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int	Shader::create_shader(const std::string &vertex_shader, const std::string &fragment_shader)
{
	unsigned int	program {glCreateProgram()};
	unsigned int	vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	unsigned int	fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void			Shader::bind() const
{
	glUseProgram(m_renderer_id);
}

void			Shader::unbind() const
{
	glUseProgram(0);
}

void			Shader::set_uniform_4f(const std::string &name, float f0, float f1, float f2, float f3)
{
	glUniform4f(get_uniform_location(name), f0, f1, f2, f3);
}

unsigned int	Shader::get_uniform_location(const std::string &name)
{
	unsigned int location = glGetUniformLocation(m_renderer_id, name.c_str());
	return location;
}
