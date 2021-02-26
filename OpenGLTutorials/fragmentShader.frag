#version 410

// uniform vec4 inputColor; // "uniform" is a variable global to all shaders that is sent from the CPU
out vec4 fragColor; // the result per fragment

void main() {
	fragColor = vec4(0.5, 0.0, 0.5, 1.0); // setting the output color according to inputColor
}