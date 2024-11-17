#version 410

layout(location = 0)in vec3 vertex_position;
layout(location = 1)in vec3 vertex_normal;
layout(location = 2)in vec4 vertex_colour;
layout(location = 3)in vec2 texture_coordinates;

uniform mat4 transform_matrix, view, projection;

out vec4 vertex_colour_out;
out vec3 vertex_normal_out, fragment_position_out;
out vec2 texture_coordinates_out;

void main() {
	
	// new version: 
	vec4 vertex_position_4 = vec4(vertex_position, 1.0);
	fragment_position_out = vec3(transform_matrix * vertex_position_4);

	// For normals, we can use a normal matrix to transform to world coords: www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/,
	// but right now we don't need to. This is only applicable if we are doing a non-uniform scale to a normal 
	vertex_normal_out = mat3(transpose(inverse(transform_matrix))) * vertex_normal;
	// vertex_normal_out = vertex_normal;
	texture_coordinates_out = texture_coordinates;

	vertex_colour_out = vertex_colour;

	gl_Position = projection * view * vec4(fragment_position_out, 1.0f);
	// gl_Position = vec4(fragment_position_out, 1.0);
}