//“ü—Í—p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txDepth : register(t2);
Texture2D txShadow : register(t3);
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
	float3 Diffuse = txDiffuse.Sample(samLinear, IN.tex).rgb;
	float3 Normal = txNormal.Sample(samLinear, IN.tex).xyz ;
	float Shadow = txShadow.Sample(samLinear, IN.tex).b;

	float3 lightVec = normalize(float3(-1, -1, 1));

	float l = max(dot(Normal, -lightVec),0.0f) * 0.5f + 0.5f;

	//if (color.a <= 0.0)discard;

	return float4(Diffuse * Shadow , 1.0f);
}