#version 410

in vec4 vt_colour;
in vec3 position_eye, normal_eye, vt_normal, frag_pos; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

uniform mat4 view;
uniform vec3 camera_pos;
vec3 light_position = vec3(0.0, 10.0, 0.0);

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
	vec4 ambience = vec4(ambience_energy * ambience_reflective, 1.0);

	// Diffuse
	vec3 light_dir = normalize(light_position - frag_pos);
	vec3 normal = normalize(vt_normal);

	// vec4 light_position_eye = vec4(view * vec4(light_position, 1.0));
	// vec3 light_source_vector = position_eye - light_position;
	// light_source_vector = normalize(light_source_vector);
	vec4 diffuse = vec4(diffuse_energy * diffuse_reflective, 1.0) *
				   max(dot(normal, light_dir), 0.0);

	// Specular
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	// vec3 light_reflection = reflect(-light_source_vector, normal_eye);
	// vec3 viewer_reflection = normalize(-position_eye);
	vec4 specular = vec4(specular_energy * specular_reflective, 1.0) *
					pow((max(dot(view_dir, reflect_dir), 0.0)), shininess_factor);


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