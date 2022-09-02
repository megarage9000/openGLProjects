#version 410
layout(location = 0)in vec3 vertex_position; // Input for our points
layout(location = 1)in vec3 vertex_colour; // Input for colors
// NOTE: layout does not work in MAC

// uniform mat4 matrix; // Our transformation matrix!
uniform mat4 matrix;
uniform mat4 view, projection;

out vec3 colour;

void main() {
	colour = vertex_colour;
	gl_Position = projection * view * matrix * vec4(vertex_position, 1.0); // Here we set the final clip-space position for each vertex position
}