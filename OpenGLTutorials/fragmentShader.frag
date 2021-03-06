#version 410

uniform vec4 inputColor; // "uniform" is a variable global to all shaders that is sent from the CPU
out vec4 fragColor; // the result per fragment

void main() {
	fragColor = inputColor; // setting the output color according to inputColor
}