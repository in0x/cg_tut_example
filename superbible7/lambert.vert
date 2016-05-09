#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 2) uniform mat4 modelTf;
layout (location = 3) uniform mat4 viewTf;
layout (location = 4) uniform mat4 projectionTf;
layout (location = 5) uniform mat4 lightModelTf = mat4(1.0);

out VS_Data // Data Type
{
	vec4 position_eye;
	vec4 light_eye;
	vec3 normal_eye;
    vec3 color;
} vs_out ; // Instance name

void main()
{
	vec3 color = vec3(190.0, 219.0, 57.0);

	mat4 modelviewTf = viewTf * modelTf;
	mat3 normalTf = inverse( transpose( mat3( modelviewTf ) ) );

	vec4 position_eye = modelviewTf * vec4( position, 1.0 );
	
	vs_out.position_eye = position_eye;
	vs_out.light_eye = viewTf * lightModelTf * vec4( 2.0, 2.0, 3.0, 1.0 );
	vs_out.normal_eye = normalize( normalTf * normal );
	vs_out.color = normalize(color);
	
	gl_Position = projectionTf * position_eye;
}