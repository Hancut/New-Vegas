#version 330
 
uniform sampler2D texture_1;
 
 in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

// Uniforms for light properties

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 color = texture2D(texture_1, texcoord);
	if (color.a < 0.5)
		discard;

	vec3 object_color = vec3(color.x, color.y, color.z);

	vec3 light_direction = vec3(0, -1, 0);
	vec3 light_position = vec3(0, 20, 0);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(world_position);
	vec3 H = normalize(L + V);
	
	float ambient_light = 0.9 * material_kd;

	float diffuse_light = material_kd * max(dot(normalize(world_normal),L),0);

	float specular_light = 0;

	//if (diffuse_light > 0)
	{
		specular_light =  pow(max(0, dot(normalize(world_normal),H)), material_shininess);
	}

	float d = length(world_position - light_position);
	float fa = 1000/pow(d,2);

	vec3 objectColor;

	objectColor = (ambient_light + (diffuse_light  + specular_light ) * fa) * object_color;

	out_color = vec4(objectColor, 1);
}