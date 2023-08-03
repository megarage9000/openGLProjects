#version 410
layout(location = 0)in vec3 vertex_position; // Input for our points
layout(location = 1)in vec3 vertex_normal; // Input for normals
layout(location = 2)in vec2 texture_coords;
layout(location = 3)in vec4 vertex_colour;
// NOTE: layout does not work in MAC

uniform mat4 matrix, view, projection;

out vec4 vt_colour;
out vec3 vt_normal, frag_pos;
out vec2 texture_coordinates;

void main() {
	
	// calculating frag position
	frag_pos = vec3(matrix * vec4(vertex_position, 1.0f));

	// For normals, we can use a normal matrix to transform to world coords: www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/,
	// but right now we don't need to. This is only applicable if we are doing a non-uniform scale to a normal
	vt_normal = mat3(transpose(inverse(matrix))) * vertex_normal;
	texture_coordinates = texture_coords;
	vt_colour = vertex_colour;
	// gl_Position = projection * vec4(position_eye, 1.0); // Here we set the final clip-space position for each vertex position
	gl_Position = projection * view * vec4(frag_pos, 1.0);
}