#ifndef _VERTEXARRAY_H_
#define _VERTEXARRAY_H_

#include "./VertexBuffer.h"
#include "./VertexBufferLayout.h"

class	VertexArray
{
private:
	unsigned int	m_renderer_id {};

public:
	VertexArray();
	~VertexArray();

	void	add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	void	bind() const;
	void	unbind() const;
};

#endif