#version 410

in vec4 vt_colour;
in vec3 vt_normal, frag_pos; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

// Material
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	
	float shininess;
};

// Light Properties
struct Light {
	vec3 position;
	vec3 direction;
	float cut_off;
	float outer_cut_off;
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;

	float constant;
	float linear;
	float quadratic;
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
	
	vec3 light_dir = normalize(light.position - frag_pos);
	
	// Ambience 
	vec4 ambience = light.ambient_colour * diffuse_data;

	// Diffuse
	vec3 normal = normalize(vt_normal);
	vec4 diffuse = light.diffuse_colour * (max(dot(normal, light_dir), 0.0) * diffuse_data);

	// Specular
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec4 specular = light.specular_colour * (specular_data *
					pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess));

	// Attenuation calculation
	float distance = length(vec3(light.position) - frag_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Applying soft cone edges
	float theta = dot(light_dir, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	fragColor = vec4(ambience + diffuse + specular);
	

}