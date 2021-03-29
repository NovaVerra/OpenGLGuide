#include "../includes/opengl_guide/VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
: m_stride {0}
{}

template<typename T>
void	VertexBufferLayout::push(unsigned int count)
{
	count += 0;
}

template<>
void	VertexBufferLayout::push<float>(unsigned int count)
{
	m_elements.push_back({GL_FLOAT, count, GL_FALSE});
	m_stride += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
}

template<>
void	VertexBufferLayout::push<unsigned int>(unsigned int count)
{
	m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
	m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_INT);
}

template<>
void	VertexBufferLayout::push<unsigned char>(unsigned int count)
{
	m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_FALSE});
	m_stride += count * VertexBufferElement::get_size_of_type(GL_UNSIGNED_BYTE);
}