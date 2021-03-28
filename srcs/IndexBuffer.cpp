#include "../includes/opengl_guide/IndexBuffer.h"
#include "../includes/opengl_guide/Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
: m_count {count}
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &m_renderer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_renderer_id);
}

void	IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
}

void	IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
