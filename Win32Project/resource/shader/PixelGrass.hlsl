//入力用
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float3 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : POSITION_SM;
	float4 Spos : TEXCOORD2;
	//float3 posEye : POSITION_EYE;
	float3 posW : POSITION_W;
};

Texture2D txDiffuse : register(t0);
Texture2D txShadow : register(t1);
SamplerState samLinear : register(s0);
SamplerState samShadow : register(s1);

float3 g_lightVec = { 1,-1,0 };

float4 main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	//IN.nrm = normalize(IN.nrm - IN.posW);

	//float3 toEye = normalize(IN.posEye - IN.posW);

	//float3 r = reflect(g_lightVec, IN.nrm);

	//float spec = pow(max(dot(toEye,r),0.0f), 200);

	float bright = max(-dot(IN.nrm, g_lightVec), 0);

	float4 diffuse = txDiffuse.Sample(samLinear, IN.tex) * lerp(float4(0.7f, 0.7f, 0.8f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f), bright );

	// ライト目線によるZ値の再算出
	//float ZValue = IN.Lpos.z / IN.Lpos.w;
	
	//// 射影空間のXY座標をテクスチャ座標に変換
	//float2 TransTexCoord;
	//TransTexCoord.x = (1.0f + IN.Lpos.x / IN.Lpos.w)*0.5f;
	//TransTexCoord.y = (1.0f - IN.Lpos.y / IN.Lpos.w)*0.5f;

	//float shadowZ = txShadow.Sample(samLinear, IN.Lpos.xy);
	//float shadow = (shadowZ + 0.001 > IN.Lpos.z) ? 1.0 : 0.5;

	float4 fogColor;
	fogColor = float4(0.5f, 0.5f, 0.6f, 0.1f);

	OUT.col = diffuse  * IN.col * IN.col.a + (1.0 - IN.col.a) * fogColor;

	return OUT.col;

}