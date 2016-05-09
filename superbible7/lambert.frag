#version 440 core

layout (location = 0) out vec4 fragColor;

const float ambient = 0.3f;
const float shininess = 1000f;

in VS_Data 
{
	vec4 position_eye;
	vec4 light_eye;
	vec3 normal_eye;
    vec3 color;
} vs_in ; 

void main()
{
	vec3 color = normalize(vs_in.color);

	// Diffuse.
	vec3 L = normalize(vs_in.light_eye.xyz - vs_in.position_eye.xyz);
	vec3 N = normalize(vs_in.normal_eye);
	float dotNL = max(dot(N, L), 0.0);

	// Specular.
	vec3 R = reflect(-L, N);
	vec3 V = -vs_in.position_eye.xyz; // Camera is at origin in view space.
	//float specular = pow(max(dot(normalize(R),normalize(V)), 0.0), shininess); 
	
	float specular = 0;
		
	fragColor = vec4(color * ambient + dotNL * color + specular * color , 1);
}
