#include "../includes/opengl_guide/header.h"
#include "../includes/opengl_guide/Renderer.h"
#include "../includes/opengl_guide/VertexBuffer.h"
#include "../includes/opengl_guide/IndexBuffer.h"
#include "../includes/opengl_guide/VertexArray.h"
#include "../includes/opengl_guide/Shader.h"
#include "../includes/opengl_guide/Texture.h"

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
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f
	};

	unsigned int	indices []
	{
		0, 1, 2,
		2, 3, 0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexArray			*va = new VertexArray {};
	VertexBuffer		*vb = new VertexBuffer {positions, 4 * 4 * sizeof(float)};
	VertexBufferLayout	*layout = new VertexBufferLayout {};
	IndexBuffer			*ib = new IndexBuffer {indices, 6};
	Shader				*shader = new Shader {"./shaders/basic.shader"};
	Texture				*texture = new Texture {"./textures/ZeroTwo.png"};

	layout->push<float>(2);
	layout->push<float>(2);
	va->add_buffer(*vb, *layout);

	shader->bind();
	shader->set_uniform_4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

	texture->bind();
	shader->set_uniform_1i("u_texture", 0);

	va->unbind();
	vb->unbind();
	ib->unbind();
	shader->unbind();

	Renderer 		*renderer = new Renderer {};

	float	r = 0.0f;
	float	increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer->clear();

		shader->bind();
		shader->set_uniform_4f("u_color", r, 0.3f, 0.8f, 1.0f);	

		renderer->draw(*va, *ib, *shader);

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

	delete vb;
	delete ib;
	delete layout;
	delete shader;
	delete texture;
	glfwTerminate();

	return 0;
}
