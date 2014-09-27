attribute vec2 uv0;

varying vec2 out_uv0;
varying vec3 oVPos;

void main()                    
{
	out_uv0 = uv0;

	gl_Position = gl_Vertex;
	oVPos	    = gl_Position.xyz;
}