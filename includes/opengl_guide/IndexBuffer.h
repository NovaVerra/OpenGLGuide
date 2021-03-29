#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

class	IndexBuffer
{
private:
	unsigned int	m_renderer_id;
	unsigned int	m_count;

public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	void	bind() const;
	void	unbind() const;

	inline unsigned int	get_count() const { return m_count; }
};

#endif
