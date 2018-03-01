//入力用
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 nrm : NORMAL0;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float4 col2: COL2;
	float4 Lpos : TEXCOORD1;
	float4 Spos : TEXCOORD2;
	float Depth : TEXCOORD5;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float main(pixcelIn IN) : SV_Target
{
	float2 Moments;

	float depth = IN.Spos.z / IN.Spos.w;

	//深度
	Moments.x = depth;

	//深度の偏微分
	float dx = ddx(depth);
	float dy = ddx(depth);

	//ピクセル範囲で計算
	Moments.y = (depth * depth) + (0.25f * (dx * dx + dy * dy));

	return Moments;
}