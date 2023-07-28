#version 410

in vec4 vt_colour;
in vec3 position_eye, normal_eye, vt_normal, frag_pos; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

// Material
struct Material {
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;
	float shininess;
};

uniform mat4 view;
uniform vec3 camera_pos, light_position;
uniform vec4 light_colour;
uniform Material material;

// Ambience
vec3 ambience_energy = vec3(0.137, 0.886, 0.922);
vec3 ambience_reflective = vec3(0.667, 0.71, 0.71);

// Diffuse
vec3 diffuse_energy = vec3(0.141, 0.812, 0.71);
vec3 diffuse_reflective = vec3(0.682, 0.941, 0.902);

// Specular
vec3 specular_energy = vec3(1.0, 1.0, 1.0);
vec3 specular_reflective = vec3(1.0, 1.0, 1.0);
float shininess_factor = 100;

out vec4 fragColor;

void main() {
	
	// Ambience 
	vec4 ambience = light_colour * material.ambient_colour;

	// Diffuse
	vec3 light_dir = normalize(light_position - frag_pos);
	vec3 normal = normalize(vt_normal);

	vec4 diffuse = light_colour * material.diffuse_colour *
				   max(dot(normal, light_dir), 0.0);

	// Specular
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec4 specular = light_colour * material.specular_colour *
					pow((max(dot(view_dir, reflect_dir), 0.0)), material.shininess);


	vec4 color = vec4(ambience + diffuse + specular);

	// vec4 color = vec4(normalize(vt_normal), 1.0);
	if(color.x < 0.0) {
		color.x = -color.x;
	}
	if(color.y < 0.0) {
		color.y = -color.y;
	}
	if(color.z < 0.0) {
		color.z = -color.z;
	}
	fragColor = color * vec4(0.5, 0.5, 0.5, 1.0);
}