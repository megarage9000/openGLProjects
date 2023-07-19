#version 410
layout(location = 0)in vec3 vertex_position; // Input for our points
layout(location = 1)in vec3 vertex_normal; // Input for normals
layout(location = 2)in vec2 texture_coords;
layout(location = 3)in vec4 vertex_colour;
// NOTE: layout does not work in MAC

uniform mat4 matrix, view, projection;

out vec4 vt_colour;
out vec3 position_eye, normal_eye, vt_normal;
out vec2 texture_coordinates;

void main() {
	
	position_eye = vec3(view * matrix * vec4(vertex_position, 1.0));
	normal_eye = vec3(view * matrix * vec4(vertex_normal, 0.0));
	vt_normal = vertex_normal;
	texture_coordinates = texture_coords;
	vt_colour = vertex_colour;
	gl_Position = projection * vec4(position_eye, 1.0); // Here we set the final clip-space position for each vertex position
}