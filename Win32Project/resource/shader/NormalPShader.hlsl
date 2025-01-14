//���͗p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 main(pixcelIn IN) : SV_Target
{
	float4 color = txDiffuse.Sample(samLinear, IN.tex) * IN.color;

	clip(color.a);

	return color;
}