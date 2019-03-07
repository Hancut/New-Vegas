#version 330

uniform sampler2D texture_1;

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color = texture2D(texture_1, texcoord);
	
	vec3 object_color = vec3(color.x, color.y, color.z);

	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	
	float ambient_light = 0.6 * material_kd;

	float diffuse_light = material_kd * max(dot(normalize(world_normal),L),0);

	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light =  pow(max(0, dot(normalize(world_normal),H)), material_shininess);
	}

	float d = length(world_position - light_position);
	float fa = 1/pow(d,2);

	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(70);

	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);

	vec3 objectColor;

	if (spot_light > cos(70)) {
		objectColor = (ambient_light + (diffuse_light  + specular_light ) * light_att_factor) * object_color * 3;
		
	} else {objectColor = ambient_light * object_color;}
	
	out_color = vec4(objectColor, 1);
}

