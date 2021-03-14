#version 410

in vec3 colour; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
out vec4 fragColor;

void main() {
	fragColor = vec4(colour, 1.0); // setting the output color according to inputColor
}