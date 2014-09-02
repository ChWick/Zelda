#version 120

//Parameters
uniform mat4 viewProjMatrix;
uniform mat4 worldMatrix;

attribute vec2 uv0;

varying vec2 out_uv0;
varying vec3 oNormal;
varying vec3 oVPos;

void main()                    
{
	vec4 worldPos		= worldMatrix * gl_Vertex;
	vec3 worldNorm		= gl_Normal * mat3(worldMatrix);


	gl_Position = viewProjMatrix * worldPos;
	out_uv0 = uv0;

	oNormal		= worldNorm;
	oVPos		= worldPos.xyz;
}