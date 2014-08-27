void main_vp(float2 uv : TEXCOORD0,
	     float4 position : POSITION,
	     float4 color : COLOR0,
 
	     out float2 oUv : TEXCOORD0,			 
	     out float4 oPosition : POSITION,
	     out float4 oColor : COLOR0,
 
	     // if these are passed as ints, then it doesn't work for some reason
	     uniform float numTilesU,
	     uniform float numTilesV,
 
	     uniform float4x4 worldViewProj)
{
	// I have no idea why, but if we don't cast these to ints then it gets screwed up
	int iNumTilesU = (int)numTilesU;
	int iNumTilesV = (int)numTilesV;
 
	int numTilesTotal = iNumTilesU * iNumTilesV;
	int selectedTile = (int)(numTilesTotal * color.a);
 
	if (selectedTile == numTilesTotal)
		selectedTile = numTilesTotal - 1;
	// the "1 - " bit is because otherwise it goes from right to left
	oUv.x = 1 - ((uv.x + selectedTile % iNumTilesU) / iNumTilesU); ///selectedTile;
	oUv.y = (uv.y + selectedTile / iNumTilesU) / iNumTilesV; ///selectedTile;
 
	// pass these on
	oPosition = mul(worldViewProj, position);
	oColor = color;
}
 
float4 main_fp (float2 uv : TEXCOORD0,
		uniform sampler2D tex : register(s0))
: COLOR
{
	return tex2D(tex, uv.xy);
}