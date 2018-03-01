//“ü—Í—p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 Out0 : SV_Target0;
	float4 Out1 : SV_Target1;
	float4 Out2 : SV_Target2;
	float4 Out3 : SV_Target3;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

PS_OUTPUT main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	PS_OUTPUT ps;

	ps.Out0 = txDiffuse.Sample(samLinear, IN.tex);
	ps.Out1 = float4(1,1,1,1);
	ps.Out2 = float4(1,1,1,1);
	ps.Out3 = float4(1,1,1,1);

	return ps;
}
