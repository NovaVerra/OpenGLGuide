#include "../includes/opengl_guide/header.h"

static void			glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool			glLogCall(const char *function, const char *file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGL Error] (" << error << "): ";
		std::cerr << function << " IN " << file << " ON LINE " << line << std::endl;
		return false;
	}
	return true;
}

ShaderSource		retrieveShader(const std::string &file_path)
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

static unsigned int	compileShader(unsigned int type, const std::string &source)
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

static unsigned int	createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	unsigned int	program {glCreateProgram()};
	unsigned int	vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int	main()
{
	GLFWwindow*	window {nullptr};

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
		std::cerr << "GLEW initialization failed" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float			positions []
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int	indices []
	{
		0, 1, 2,
		2, 3, 0
	};

	/*
	It is likely that without the window hints it was defaulting to the COMPAT profile of opengl rather than CORE. COMPAT has a default VAO but in the CORE profile a VAO is required to be explicitly created and bound if you bound the VAO after calling glVertexAttribPointer then that would cause an error because the function is a VAO state changer and requires a VAO to bound
	*/
	unsigned int va_id;
	glGenVertexArrays(1, &va_id);
	glBindVertexArray(va_id);

	/* Gave OpenGL the buffer/data for the triangle */
	unsigned int	buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
	
	/* Gave OpenGL Vertex Attribute Layout */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	/* Gave OpenGL the buffer/data for the triangle */
	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	ShaderSource	source {retrieveShader("./shaders/basic.shader")};
	unsigned int	shader {createShader(source.vertexSource, source.fragmentSource)};
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// This by itself does not render - non shaders yet
		glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}
