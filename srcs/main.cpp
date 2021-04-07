#include "../includes/opengl_guide/header.h"
#include "../includes/opengl_guide/Renderer.h"
#include "../includes/opengl_guide/VertexBuffer.h"
#include "../includes/opengl_guide/IndexBuffer.h"
#include "../includes/opengl_guide/VertexArray.h"
#include "../includes/opengl_guide/Shader.h"
#include "../includes/opengl_guide/Texture.h"
#include "../includes/opengl_guide/vendor/glm/glm.hpp"
#include "../includes/opengl_guide/vendor/glm/gtc/matrix_transform.hpp"
#include "../includes/opengl_guide/vendor/imgui/imgui.h"
#include "../includes/opengl_guide/vendor/imgui/imgui_impl_glfw_gl3.h"

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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
		100.0f, 100.0f, 0.0f, 0.0f,
		200.0f, 100.0f, 1.0f, 0.0f,
		200.0f, 200.0f, 1.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f
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
	Shader				*shader = new Shader {"./res/shaders/basic.shader"};
	Texture				*texture = new Texture {"./res/textures/ZeroTwo.png"};
	Renderer 			*renderer = new Renderer {};

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

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

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark(); 

	glm::vec3 translation(200, 200, 0);

	float	r = 0.0f;
	float	increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer->clear();

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 mvp = proj * view * model;

		shader->bind();
		shader->set_uniform_4f("u_color", r, 0.3f, 0.8f, 1.0f);
		shader->set_uniform_matrix_4f("u_mvp", mvp);

		renderer->draw(*va, *ib, *shader);

		if (r > 1.0f)
			increment  = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;

		{
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	delete vb;
	delete ib;
	delete layout;
	delete shader;
	delete texture;
	glfwTerminate();

	return 0;
}
