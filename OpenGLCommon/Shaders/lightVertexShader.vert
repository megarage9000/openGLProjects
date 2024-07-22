#version 410

layout(location = 0)in vec3 vertex_position;

uniform mat4 matrix, view, projection;

void main() {
	vec3 position_eye = vec3(view * matrix * vec4(vertex_position, 1.0));
	gl_Position = projection * vec4(position_eye, 1.0);
}