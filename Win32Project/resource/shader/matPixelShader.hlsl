struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float2 tex2 : TEX2;
};

struct PS_OUTPUT
{
	float4 Out0 : SV_Target0;
	float4 Out1 : SV_Target1;
	float4 Out2 : SV_Target2;
	float4 Out3 : SV_Target3;
};

float3 g_lightVec = { 0.71,-0.71,0.0 };
Texture2D txDiffuse2 : register(t1);
SamplerState samLinear : register(s0);

PS_OUTPUT main(pixcelIn IN) : SV_Target
{
	PS_OUTPUT ps;

	pixcelIn OUT;
	
	float bright = max(dot(IN.nrm, g_lightVec), 0);
	
	float4 shadow = lerp(float4(0.7f, 0.7f, 0.8f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), bright);
	
	float4 diffuse = txDiffuse2.Sample(samLinear, IN.tex2) * saturate(IN.col) * shadow;
	
	OUT.col = diffuse;

	ps.Out0 = OUT.col;
	ps.Out1.rgb = float3(1,1,1);
	ps.Out2 = float4(1, 1, 1, 1);
	ps.Out3.rgb = float3(1, 1, 1);

	return ps;
}