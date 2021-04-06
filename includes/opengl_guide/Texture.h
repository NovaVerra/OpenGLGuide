#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "./Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int	m_renderer_id;
	std::string		m_file_path;
	unsigned char	*m_local_buffer;

	int				m_width;
	int				m_height;
	int				m_bpp;

public:
	Texture(const std::string &file_path);
	~Texture();

	void	bind(unsigned int slot = 0) const;
	void	unbind() const;
};

#endif
