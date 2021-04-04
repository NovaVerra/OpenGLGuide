#include "../includes/opengl_guide/header.h"
#include "../includes/opengl_guide/Renderer.h"
#include "../includes/opengl_guide/VertexBuffer.h"
#include "../includes/opengl_guide/IndexBuffer.h"
#include "../includes/opengl_guide/VertexArray.h"

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

	VertexArray			*va = new VertexArray {};
	VertexBuffer		*vb = new VertexBuffer {positions, 8 * sizeof(float)};
	VertexBufferLayout	*layout = new VertexBufferLayout {};
	layout->push<float>(2);
	va->add_buffer(*vb, *layout);

	IndexBuffer		*ib = new IndexBuffer {indices, 6};

	ShaderSource	source {retrieveShader("./shaders/basic.shader")};
	unsigned int	shader {createShader(source.vertexSource, source.fragmentSource)};
	glUseProgram(shader);

	/* in order to send uniform data, a shader HAS to be bound */
	glCall(int	location = glGetUniformLocation(shader, "u_color"));
	ASSERT(location != -1)
	glCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

	va->unbind();
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float	r = 0.0f;
	float	increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		// This by itself does not render - non shaders yet
		glCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		
		va->bind();
		ib->bind();

		glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0f)
			increment  = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;
	
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	delete vb;
	delete ib;
	delete layout;
	glfwTerminate();

	return 0;
}
