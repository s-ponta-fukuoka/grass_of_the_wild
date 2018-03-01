//入力用
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float2 tex2 : TEX2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
	float2 fogFactor : FOG;
};

struct PS_OUTPUT
{
	float4 Out0 : SV_Target0;
	float4 Out1 : SV_Target1;
	float4 Out2 : SV_Target2;
	float4 Out3 : SV_Target3;
};

Texture2D txDiffuse : register(t0);
Texture2D txDiffuse2 : register(t1);
Texture2D txBayer : register(t2);
Texture2D txShadow : register(t3);

SamplerState samLinear : register(s0);

float3 g_lightVec = { 0.71,-0.71,0.0 };

PS_OUTPUT main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	PS_OUTPUT ps;

	float bright = dot(IN.nrm, -float3(0,-1,1));
	
	bright = bright * 0.5f + 0.5;

	bright = bright * bright;
	
	float4 diffuse =txDiffuse.Sample(samLinear, IN.tex) * lerp(IN.col, float4(1.0f, 1.0f, 1.0f, 1.0f), bright);

	// ライト目線によるZ値の再算出
	float ZValue = IN.Lpos.z / IN.Lpos.w;

	// 射影空間のXY座標をテクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + IN.Lpos.x / IN.Lpos.w)*0.5f;
	TransTexCoord.y = (1.0f - IN.Lpos.y / IN.Lpos.w)*0.5f;

	float2 Moments = txShadow.Sample(samLinear, IN.Lpos.xy).xy;

	float shadow = (Moments.x + 0.001 > IN.Lpos.z) ? 1.0 : 0.5;

	if (diffuse.a <= 0.0)discard;

	float4 fogColor;
	fogColor = float4(0.5f, 0.5f, 0.6f, 0.1f);

	float2 ditherUv = IN.pos.xy / 4;

	float dither = txBayer.Sample(samLinear, ditherUv).r;
	clip(IN.fogFactor.x - dither);

	ps.Out0 = IN.col * txDiffuse.Sample(samLinear, IN.tex) * txDiffuse2.Sample(samLinear, float2(bright, 0.0f))  * IN.fogFactor.y + (1.0 - IN.fogFactor.y) * fogColor;
	ps.Out1.rgb = IN.nrm.rgb * 0.5f + 0.5f;
	ps.Out2 = 0;
	ps.Out3.rgb = float3(1, 1, 1);

	return ps;
}