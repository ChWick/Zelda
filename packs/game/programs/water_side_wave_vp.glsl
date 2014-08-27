uniform mat4 worldViewProj;
uniform float numTilesU;
uniform float numTilesV;
uniform int selectedTile;
attribute vec2 uv0;

varying vec2 out_uv0;

void main()                    
{
	gl_Position = worldViewProj * gl_Vertex;

	// I have no idea why, but if we don't cast these to ints then it gets screwed up
	int iNumTilesU = int(numTilesU);
	int iNumTilesV = int(numTilesV);

	int iTilesPart = selectedTile / iNumTilesU;

	// manual modulo operator x - int(x/a) * a
	out_uv0.x = uv0.x + ((float(selectedTile - iTilesPart * iNumTilesU)) / float(iNumTilesU)); 	out_uv0.y = uv0.y + ((float(iTilesPart)) / float(iNumTilesV));

}