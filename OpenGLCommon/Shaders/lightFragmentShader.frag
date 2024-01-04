#version 410
out vec4 FragColor;

uniform vec4 light_colour;

void main() {
	FragColor = light_colour;
}
