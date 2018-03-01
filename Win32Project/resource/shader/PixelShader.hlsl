//“ü—Í—p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
	float fogFactor : FOG;
	float3 posEye : TEXCOORD3;
	float3 posW : TEXCOORD4;
	float Depth : TEXCOORD5;
};

struct PS_OUTPUT
{
	float4 Out0 : SV_Target0;
	float4 Out1 : SV_Target1;
	float4 Out2 : SV_Target2;
	float4 Out3 : SV_Target3;
};

Texture2D txDiffuse : register(t0);
Texture2D txShadow : register(t1);
SamplerState samLinear : register(s0);
SamplerState samShadow : register(s1);

float3 g_lightVec;

PS_OUTPUT main(pixcelIn IN) : SV_Target
{
	PS_OUTPUT ps;

	pixcelIn OUT;

	IN.nrm = normalize(IN.nrm);
	
	float3 lightVec = normalize(float3(-1, -1, 1));

	float3 toEye = normalize(IN.posEye - IN.posW);
	
	float3 r = reflect(lightVec, IN.nrm);
	
	float spec = pow(max(dot(toEye, r), 0.0f), 10);
	
	float l = (dot(IN.nrm, -lightVec) * 0.5) + 0.5;
	
	float4 diffuse = float4(l, l, l, 1.0f);

	float2 Moments = txShadow.Sample(samLinear, IN.Lpos.xy).xy;

	float shadow = (Moments.x + 0.001 > IN.Lpos.z) ? 1.0 : 0.5;

	float4 fogColor;
	fogColor = float4(0.5f, 0.5f, 0.6f, 0.1f);

	ps.Out0 = txDiffuse.Sample(samLinear, IN.tex) * IN.col;
	ps.Out1.rgb = IN.nrm.xyz;
	ps.Out2 = IN.Spos.z / IN.Spos.w;
	ps.Out3.rgb = float3(1, 1, shadow);

	return ps;
}

float4 ps_main2(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	OUT.col = saturate(IN.col2) * IN.col;

	return OUT.col;
}