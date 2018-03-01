//“ü—Í—p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

static const float BlurWeights[13] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};

float4 main(pixcelIn IN) : SV_Target
{
	float2 offsetUV = float2(1.0f / (1280.0f / 16), 0.0f);
	float2 offsetStart = -offsetUV * 6;

	float4 Color = float4(0, 0, 0, 0);

	for (int i = 0; i < 13; i++)
	{
		float3 ColorOut = txDiffuse.Sample(samLinear, IN.tex + offsetStart + offsetUV * i).rgb * BlurWeights[i];
		Color += float4(ColorOut * 1.5f, 1.0f);
	}

	return Color;
}