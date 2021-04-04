#include "../includes/opengl_guide/Renderer.h"
#include <iostream>

void	glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool	glLogCall(const char *function, const char *file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGL Error] (" << error << "): ";
		std::cerr << function << " IN " << file << " ON LINE " << line << std::endl;
		return false;
	}
	return true;
}

void	Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.bind();
	va.bind();
	ib.bind();
	glCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}

void	Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}