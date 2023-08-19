
#define MAX_NUM_LIGHTS_PER_TYPE 10

in vec3 vt_normal, frag_pos
in vec2 texture_coordinates;

// Directional Lighting
struct DirectionalLight {
	vec3 direction;
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;
};

vec4 calculate_directional_light(DirectionalLight light, vec3 normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data);

// Point Lighting 
struct PointLight {
	vec4 position;
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;

	float constant;
	float linear;
	float quadratic;
};
vec4 calculate_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data);

// Spot Lighting 
struct SpotLight {
	vec4 position;
	vec4 ambient_colour;
	vec4 diffuse_colour;
	vec4 specular_colour;
	vec3 direction;

	float constant;
	float linear;
	float quadratic;
};
vec4 calculate_spot_light(SpotLight light, vec3 vt_normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data);

// Material
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	
	float shininess;
};

// Uniforms
uniform Mat4 view;
uniform vec3 camera_pos;
uniform Material material;

uniform DirectionalLight dir_lights[MAX_NUM_LIGHTS_PER_TYPE];
uniform PointLight point_lights[MAX_NUM_LIGHTS_PER_TYPE];
uniform SpotLight spot_lights[MAX_NUM_LIGHTS_PER_TYPE];

uniform int num_dir_lights;
uniform int num_point_lights;
uniform int num_spot_lights;

out fragColor;

void main() {
	vec4 diffuse_data = vec4(texture(material.diffuse, texture_coordinates).rgb, 1.0f);
	vec4 specular_data = vec4(texture(material.specular, texture_coordinates).rgb, 1.0f);

	num_dir_lights = num_dir_lights > MAX_NUM_LIGHTS_PER_TYPE ? MAX_NUM_LIGHTS_PER_TYPE : num_dir_lights;
	num_point_lights = num_point_lights > MAX_NUM_LIGHTS_PER_TYPE ? MAX_NUM_LIGHTS_PER_TYPE : num_point_lights;
	num_spot_lights = num_spot_lights > MAX_NUM_LIGHTS_PER_TYPE ? MAX_NUM_LIGHTS_PER_TYPE : num_spot_lights;

	vec4 result = vec4(0.0, 0.0, 0.0, 0.0);
	// Calculate directional lights
	for(int i = 0; i < num_dir_lights; i++) {
		result += calculate_directional_light(dir_lights[i], vt_normal, frag_pos, camera_pos, diffuse_data, specular_data);
	}
	// Calculate point lights
	for(int i = 0; i < num_point_lights; i++) {
		result += calculate_point_light(point_lights[i], vt_normal, frag_pos, camera_pos, diffuse_data, specular_data);
	}
	// Calculate spot lights
	for(int i = 0; i < num_spot_lights; i++) {
		result += calculate_spot_light(spot_lights[i], vt_normal, frag_pos, camera_pos, diffuse_data, specular_data);
	}
	fragColor = result;
}

// Calculating directional light
vec4 calculate_directional_light(DirectionalLight light, vec3 vt_normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data) {

	// Ambience 
	vec4 ambience = light.ambient_colour * diffuse_data;

	// Diffuse
	vec3 light_dir = normalize(-light.direction);
	vec3 normal = normalize(vt_normal);

	vec4 diffuse = light.diffuse_colour * (diffuse_data *
				   max(dot(normal, light_dir), 0.0));

	// Specular
	vec3 view_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	vec4 specular = light.specular_colour * (specular_data *
					pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess));

	return vec4(ambience + diffuse + specular);
}

vec4 calculate_point_light(PointLight light, vec3 vt_normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data) {

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

	// Calculating Attenuation
	float distance = length(vec3(light.position) - frag_pos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambience *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec4(ambience + diffuse + specular);
}

// Calculating Spot light
vec4 calculate_spot_light(SpotLight light, vec3 vt_normal, vec3 frag_pos, vec3 camera_pos, vec4 diffuse_data, vec4 specular_data) {
	
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

	return vec4(ambience + diffuse + specular);
}