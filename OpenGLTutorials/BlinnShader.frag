#version 410

in vec3 position_eye, normal_eye, vt_normal; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

uniform mat4 view;
uniform vec3 light_position;

// Ambience
vec3 ambience_energy = vec3(0.137, 0.886, 0.922);
vec3 ambience_reflective = vec3(0.667, 0.71, 0.71);


// Diffuse
vec3 diffuse_energy = vec3(0.141, 0.812, 0.71);
vec3 diffuse_reflective = vec3(0.682, 0.941, 0.902);

// Specular
vec3 specular_energy = vec3(1.0, 1.0, 1.0);
vec3 specular_reflective = vec3(0.75, 0.75, 0.75);
float shininess_factor = 20;

out vec4 fragColor;

void main() {
	
	// Ambience 
	vec4 ambience = vec4(ambience_energy * ambience_reflective, 1.0);

	// Diffuse
	vec4 light_position_eye = vec4(view * vec4(light_position, 1.0));
	vec3 light_source_vector = position_eye - light_position;
	light_source_vector = normalize(light_source_vector);
	vec4 diffuse = vec4(diffuse_energy * diffuse_reflective, 1.0) *
				   max(dot(light_source_vector, normal_eye), 0.0);

	// Specular
	vec3 light_reflection = reflect(-light_source_vector, normal_eye);
	vec3 viewer_reflection = normalize(-position_eye);
	vec4 specular = vec4(specular_energy * specular_reflective, 1.0) *
					pow((max(dot(light_reflection, viewer_reflection), 0.0)), shininess_factor);


	fragColor = vec4(diffuse + ambience + specular);

}