uniform sampler2D tex0;

varying vec2 out_uv0;

void main()
{
	gl_FragColor = texture2D(tex0, out_uv0);
}

