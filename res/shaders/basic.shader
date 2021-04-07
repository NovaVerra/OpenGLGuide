#shader	vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;

out vec2 v_texture_coord;

uniform mat4 u_mvp;

void main()
{
	gl_Position = position * u_mvp;
	v_texture_coord = texture_coord;
}

#shader	fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texture_coord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
	vec4 texture_color = texture(u_texture, v_texture_coord);
	color = texture_color;
}
