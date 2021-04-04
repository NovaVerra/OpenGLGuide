#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <unordered_map>

struct	ShaderSource
{
	std::string	vertexSource {};
	std::string	fragmentSource {};
};

class	Shader
{
private:
	std::string								file_path;
	unsigned int							m_renderer_id;
	std::unordered_map<std::string, int>	m_uniform_location_cache;
	// caching for uniform
public:
	Shader(const std::string &file_path);
	~Shader();

	void	bind() const;
	void	unbind() const;

	// Set uniforms
	void	set_uniform_4f(const std::string &name, float f0, float f1, float f2, float f3);

private:
	ShaderSource	retrieve_shader(const std::string &file_path);
	unsigned int	compile_shader(unsigned int type, const std::string &source);
	unsigned int	create_shader(const std::string &vertex_shader, const std::string &fragment_shader);
	int				get_uniform_location(const std::string &name);
};

#endif
