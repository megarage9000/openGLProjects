#version 410

in vec4 vertex_colour_out;
in vec3 vertex_normal_out, fragment_position_out;
in vec2 texture_coordinates_out;

// TODO, create a uniform system that accepts textures
uniform sampler2D specular_samplers[6];
uniform int specular_set[6];
uniform sampler2D diffuse_samplers[6];
uniform int diffuse_set[6];

out vec4 FragColor;

void main() 
{
	vec3 diffuse = vec3(1.0f, 1.0f, 1.0f);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);

	for(int i = 0; i < 6; i++) {
		
		if(specular_set[i] == 1) {
			specular = specular * vec3(texture(specular_samplers[i], texture_coordinates_out));
		}
		if(diffuse_set[i] == 1) {
			diffuse = diffuse * vec3(texture(diffuse_samplers[i], texture_coordinates_out));
		}
	}
	FragColor = vec4(diffuse + specular, 1.0f);

}