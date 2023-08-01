#version 410

in vec4 vt_colour;
in vec3 position_eye, normal_eye, vt_normal, frag_pos; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

// Material
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	
	float shininess;
};

// Light Properties
struct Light {
	vec4 position;
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;
};

uniform mat4 view;
uniform vec3 camera_pos;
uniform Material material;
uniform Light light;

out vec4 fragColor;

void main() {

	vec4 diffuse_data = vec4(texture(material.diffuse, texture_coordinates).rgb, 1.0f);
	vec4 specular_data = vec4(texture(material.specular, texture_coordinates).rgb, 1.0f);
	// vec4 specular_data = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Ambience 
	vec4 ambience = light.ambient_colour * diffuse_data;

	// Diffuse
	vec3 light_dir = normalize(vec3(light.position) - frag_pos);
	vec3 normal = normalize(vt_normal);

	vec4 diffuse = light.diffuse_colour * (diffuse_data *
				   max(dot(normal, light_dir), 0.0));

	// Specular
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec4 specular = light.specular_colour * (specular_data *
					pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess));


	vec4 color = vec4(ambience + diffuse + specular);

	// vec4 color = vec4(normalize(vt_normal), 1.0);
	/*
	if(color.x < 0.0) {
		color.x = -color.x;
	}
	if(color.y < 0.0) {
		color.y = -color.y;
	}
	if(color.z < 0.0) {
		color.z = -color.z;
	}*/
	fragColor = color;
}