#version 410

in vec4 vertex_colour;
in vec3 vertex_normal_out, fragment_position_out;
in vec2 texture_coordinates_out;

// TODO, create a uniform system that accepts textures
struct Material {
	sampler2D specular;
	sampler2D diffuse;
	sampler2D normal;
};

uniform Material[32] material;

out vec4 FragColor;

void main() 
{
	FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}