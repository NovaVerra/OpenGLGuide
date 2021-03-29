#ifndef _VERTEXBUFFERLAYOUT_H_
#define _VERTEXBUFFERLAYOUT_H_

#include <vector>
#include <GL/glew.h>

struct	VertexBufferElement
{
	unsigned int	type;
	unsigned int	count;
	unsigned int	normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};


class	VertexBufferLayout
{
private:
	std::vector<VertexBufferElement>	m_elements {};
	unsigned int						m_stride {};

public:
	VertexBufferLayout();
	~VertexBufferLayout() = default;

	template <typename T>
	void	push(unsigned int count);

	template<>
	void	push<float>(unsigned int count);
	template<>
	void	push<unsigned int>(unsigned int count);
	template<>
	void	push<unsigned char>(unsigned int count);

	const std::vector<VertexBufferElement> get_elements() const { return m_elements; }
	inline unsigned int	get_stride() const { return m_stride; }
};

#endif
