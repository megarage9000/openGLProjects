#version 410

in vec3 position_eye, normal_eye, vt_normal; // The input variable must have the same name as the one from vertex shader if you are using one from the vertex!
in vec2 texture_coordinates;

uniform mat4 view;

// texture for 4.2
// layout(binding = 0) uniform sampler2D basic_texture;
uniform sampler2D basic_texture;

// For texture
vec4 texel = texture(basic_texture, texture_coordinates);
vec3 texture_colour = texel.xyz;

// light position
vec3 light_position = vec3(0.0, 0.0, 0.2);
vec3 light_spec = vec3(1.0, 1.0, 1.0); // white specular colour
vec3 light_diff = vec3(0.7, 0.7, 0.7); // dull white diffuse colour
vec3 light_ambi = vec3(0.2, 0.2, 0.2); // grey ambient colour

// surface properties
vec3 reflec_spec = vec3(1.0, 1.0, 1.0); // fully reflect specular light
vec3 reflec_diff = vec3(0.95, 0.48, 0.043); // orange diffuse surface reflectance
vec3 reflec_ambi = vec3(1.0, 1.0, 1.0); // fully reflect ambient light
float spec_exponent = 100.0; // specular power

out vec4 fragColor;

void main() {
	// ambient intensity
	vec3 ambi_intensity = light_ambi * reflec_ambi;

	// diffuse intensity 
	/*
		1. We calculate the distance between the surface and the light source,
		2. We get the normailzed direction from 1.
		3. We do a dot product between the surface normal and normalized direction
		4. Ensure that it isn't < 0 so we max.
	*/
	vec3 light_position_eye = vec3(view * vec4(light_position, 1.0));
	vec3 dist_to_light_eye = light_position_eye - position_eye;
	vec3 dir_to_light_eye = normalize(dist_to_light_eye);
	float dot_prod = dot(dir_to_light_eye, normal_eye);
	dot_prod = max(dot_prod, 0.0);
	vec3 diff_intensity = light_diff * reflec_diff * dot_prod; // final diffuse intensity

	// specular intensity
	/*
		1. 
	*/

	vec3 surface_to_viewer_eye = normalize(-position_eye);
	// Phong 
/*
	vec3 reflection_eye = reflect(-dir_to_light_eye, normal_eye);
	float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);
	dot_prod_specular = max(dot_prod_specular, 0.0);
*/
	// Blinn
	vec3 half_way_eye = normalize(surface_to_viewer_eye + dir_to_light_eye);
	float dot_prod_specular = max(0.0, dot(half_way_eye, normal_eye));

	float spec_factor = pow(dot_prod_specular, spec_exponent);
	vec3 spec_intensity = light_spec * reflec_ambi * spec_factor;

	// final colour 
	fragColor = vec4(spec_intensity + diff_intensity + ambi_intensity, 1.0);
	//fragColor = vec4(vt_normal, 1.0);

	// fragColor = texel;
}