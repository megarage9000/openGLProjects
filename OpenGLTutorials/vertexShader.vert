#version 410
layout(location = 0)in vec3 vertex_position; // Input for our points
layout(location = 1)in vec3 vertex_normal; // Input for colors
// NOTE: layout does not work in MAC

uniform mat4 matrix, view, projection;

out vec3 position_eye, normal_eye;

void main() {
	
	position_eye = vec3(view * matrix * vec4(vertex_position, 1.0));
	normal_eye = vec3(view * matrix * vec4(vertex_normal, 0.0));
	gl_Position = projection * vec4(position_eye, 1.0); // Here we set the final clip-space position for each vertex position
}