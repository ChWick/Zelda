uniform sampler2D tex0;

uniform vec4	lightPosition;
uniform vec3	cameraPosition;
uniform vec3 	lightAmbient;
uniform vec3	lightDiffuse;
uniform vec3	lightSpecular;
uniform vec4	lightAttenuation;
uniform float	lightGloss;

uniform float	blinking_intensity;
uniform float	blinking_colour;

varying vec2 out_uv0;
varying vec3 oNormal;
varying vec3 oVPos;

void main()
{
	vec4 baseColour = texture2D( tex0, out_uv0 );

	//Blinn-Phong lighting
	vec3 normal	= normalize( oNormal );
	vec3 lightDir		= lightPosition.xyz - oVPos * lightPosition.w;
	vec3 eyeDir			= normalize( cameraPosition - oVPos );

	float fLength	= length( lightDir );
	lightDir			= normalize( lightDir );

	float NdotL	= max( 0.0, dot( normal, lightDir ) );
	vec3 halfVector		= normalize(lightDir + eyeDir);
	float HdotN	= max( 0.0, dot( halfVector, normal ) );
	
	vec3 ambient  = lightAmbient * baseColour.xyz;
	vec3 diffuse  = lightDiffuse * NdotL * baseColour.xyz;
	vec3 specular = lightSpecular * pow( HdotN, lightGloss );
	
	vec3 directLighting = (diffuse + specular);

	gl_FragColor = vec4( directLighting + ambient, baseColour.a );
	
	if (blinking_intensity > 0.0) {
		float scaled_blinking_colour = blinking_colour;
		gl_FragColor = (gl_FragColor + blinking_intensity * vec4(1.0 - scaled_blinking_colour, max(0.0, 1.0 - 4.0 * scaled_blinking_colour), scaled_blinking_colour, 1.0)) / (1.0 + blinking_intensity);
	}
	
}

