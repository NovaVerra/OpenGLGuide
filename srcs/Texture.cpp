#include "../includes/opengl_guide/Texture.h"
#include "../includes/opengl_guide/VStbImage.h"

Texture::Texture(const std::string &file_path)
: m_renderer_id {0}, m_file_path {file_path}, m_local_buffer {nullptr}, m_width {0}, m_height {0}, m_bpp {0}
{
	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(m_file_path.c_str(), &m_width, &m_height, &m_bpp, 4);
	glCall(glGenTextures(1, &m_renderer_id));
	glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_local_buffer)
		stbi_image_free(m_local_buffer);
}

Texture::~Texture()
{
	glCall(glDeleteTextures(1, &m_renderer_id));
}

void	Texture::bind(unsigned int slot) const
{
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void	Texture::unbind() const
{
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}
