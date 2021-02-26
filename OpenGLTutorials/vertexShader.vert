#version 410

in vec3 vertex_position; // Input for our vertex shader

void main() {
	gl_Position = vec4(vertex_position, 1.0); // Here we set the final clip-space position for each vertex position
}