//“ü—Í—p
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float3 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
	float3 posEye : POSITION_EYE;
	float3 posW : POSITION_W;
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
Texture2D txBayer : register(t2);
SamplerState samLinear : register(s0);
SamplerState samShadow : register(s1);

float3 g_lightVec = { 1,-1,0 };

PS_OUTPUT main(pixcelIn IN) : SV_Target
{
	PS_OUTPUT ps;

	pixcelIn OUT;

	IN.nrm = normalize(IN.nrm);

	float3 lightVec = normalize(float3(-1, -1, 1));

	float3 toEye = normalize(IN.posEye - IN.posW);
	
	float3 r = reflect(lightVec, IN.nrm);

	float spec = pow(max(dot(toEye,r),0.0f), 10);

	float l = (dot(IN.nrm, -lightVec) * 0.5) + 0.5 * 0.8;

	float4 diffuse = float4(l,l,l,1.0f);

	float4 fogColor;
	fogColor = float4(0.5f, 0.5f, 0.6f, 0.1f);

	float2 ditherUv = IN.pos.xy / 4;

	float dither = txBayer.Sample(samLinear, ditherUv).r;

	clip(IN.col.a - dither);

	ps.Out0 = (diffuse) * IN.col;
	ps.Out1.rgb = IN.nrm.xyz * 0.5f + 0.5f;
	ps.Out2 = IN.Spos.z / IN.Spos.w;
	ps.Out3.rgb = float3(1, 1, 1);

	return ps;

}